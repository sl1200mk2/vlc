/*****************************************************************************
 * interface_widgets.cpp : Custom widgets for the main interface
 ****************************************************************************
 * Copyright (C) 2006 the VideoLAN team
 * $Id$
 *
 * Authors: Clément Stenac <zorglub@videolan.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#include "dialogs_provider.hpp"
#include <vlc/vout.h>
#include "qt4.hpp"
#include "components/interface_widgets.hpp"
#include "main_interface.hpp"
#include "input_manager.hpp"
#include <QHBoxLayout>

#define ICON_SIZE 128

/**********************************************************************
 * Video Widget. A simple frame on which video is drawn
 * This class handles resize issues
 **********************************************************************/
static void *DoRequest( intf_thread_t *, vout_thread_t *, int*,int*,
                        unsigned int *, unsigned int * );
static void DoRelease( intf_thread_t *, void * );
static int DoControl( intf_thread_t *, void *, int, va_list );

bool need_update;

VideoWidget::VideoWidget( intf_thread_t *_p_i ) : QFrame( NULL ), p_intf( _p_i )
{
    vlc_mutex_init( p_intf, &lock );

    p_intf->pf_request_window  = ::DoRequest;
    p_intf->pf_release_window  = ::DoRelease;
    p_intf->pf_control_window  = ::DoControl;
    p_intf->p_sys->p_video = this;
    p_vout = NULL;

    setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred );

    ON_TIMEOUT( update() );
    need_update = false;
}

void VideoWidget::update()
{
    if( need_update )
    {
        p_intf->p_sys->p_mi->resize( p_intf->p_sys->p_mi->sizeHint() );
        need_update = false;
    }
}

VideoWidget::~VideoWidget()
{
    vlc_mutex_lock( &lock );
    if( p_vout )
    {
        if( !p_intf->psz_switch_intf )
        {
            if( vout_Control( p_vout, VOUT_CLOSE ) != VLC_SUCCESS )
                vout_Control( p_vout, VOUT_REPARENT );
        }
        else
        {
            if( vout_Control( p_vout, VOUT_REPARENT ) != VLC_SUCCESS )
                vout_Control( p_vout, VOUT_CLOSE );
        }
    }
    p_intf->pf_request_window = NULL;
    p_intf->pf_release_window = NULL;
    p_intf->pf_control_window = NULL;
    vlc_mutex_unlock( &lock );
    vlc_mutex_destroy( &lock );
}

QSize VideoWidget::sizeHint() const
{
    return widgetSize;
}

static void *DoRequest( intf_thread_t *p_intf, vout_thread_t *p_vout,
                        int *pi1, int *pi2, unsigned int*pi3,unsigned int*pi4)
{
    return p_intf->p_sys->p_video->Request( p_vout, pi1, pi2, pi3, pi4 );
}

void *VideoWidget::Request( vout_thread_t *p_nvout, int *pi_x, int *pi_y,
                           unsigned int *pi_width, unsigned int *pi_height )
{
    if( p_vout )
    {
        msg_Dbg( p_intf, "embedded video already in use" );
        return NULL;
    }
    p_vout = p_nvout;

    setMinimumSize( 1,1 );
    widgetSize = QSize( *pi_width, *pi_height );
    updateGeometry();
    need_update = true;
    return  (void*)winId();
}

static void DoRelease( intf_thread_t *p_intf, void *p_win )
{
    return p_intf->p_sys->p_video->Release( p_win );
}

void VideoWidget::Release( void *p_win )
{
    p_vout = NULL;
    if( config_GetInt( p_intf, "qt-always-video" ) == 0 )
    {
        widgetSize = QSize ( 1,1 );
        updateGeometry();
        need_update = true;
    }
}

static int DoControl( intf_thread_t *p_intf, void *p_win, int i_q, va_list a )
{
    return p_intf->p_sys->p_video->Control( p_win, i_q, a );
}

int VideoWidget::Control( void *p_window, int i_query, va_list args )
{
    int i_ret = VLC_EGENERIC;
    vlc_mutex_lock( &lock );
    switch( i_query )
    {
        case VOUT_GET_SIZE:
        {
            unsigned int *pi_width  = va_arg( args, unsigned int * );
            unsigned int *pi_height = va_arg( args, unsigned int * );
            *pi_width = frame->width();
            *pi_height = frame->height();
            i_ret = VLC_SUCCESS;
            break;
        }
        case VOUT_SET_SIZE:
        {
            unsigned int i_width  = va_arg( args, unsigned int );
            unsigned int i_height = va_arg( args, unsigned int );

            if( !i_width && p_vout ) i_width = p_vout->i_window_width;
            if( !i_height && p_vout ) i_height = p_vout->i_window_height;
            widgetSize = QSize( i_width, i_height );
            updateGeometry();
            need_update = true;
            i_ret = VLC_SUCCESS;
            break;
        }
        case VOUT_SET_STAY_ON_TOP:
        {
            /// \todo
            break;
        }
        default:
            msg_Warn( p_intf, "unsupported control query" );
            break;
    }
    vlc_mutex_unlock( &lock );
    return i_ret;
}

/**********************************************************************
 * Background Widget. Show a simple image background. Currently,
 * it's a static cone.
 **********************************************************************/
BackgroundWidget::BackgroundWidget( intf_thread_t *_p_i ) :
                                        QFrame( NULL ), p_intf( _p_i )
{
    DrawBackground();
    CONNECT( THEMIM->getIM(), audioStarted(), this, hasAudio() );
    CONNECT( THEMIM->getIM(), audioStarted(), this, hasVideo() );
}

int BackgroundWidget::DrawBackground()
{
   setAutoFillBackground( true );
    plt =  palette();
    plt.setColor( QPalette::Active, QPalette::Window , Qt::black );
    plt.setColor( QPalette::Inactive, QPalette::Window , Qt::black );
    setPalette( plt );

    backgroundLayout = new QHBoxLayout;
    label = new QLabel( "" );
    label->setMaximumHeight( ICON_SIZE );
    label->setMaximumWidth( ICON_SIZE );
    label->setScaledContents( true );
    label->setPixmap( QPixmap( ":/vlc128.png" ) );
    backgroundLayout = new QHBoxLayout;
    backgroundLayout->addWidget( label );
    setLayout( backgroundLayout );
    return 0;
}

int BackgroundWidget::CleanBackground()
{
    backgroundLayout->takeAt(0);
    delete backgroundLayout;
    return 0;
}

QSize BackgroundWidget::sizeHint() const
{
    return widgetSize;
}

void BackgroundWidget::hasAudio()
{
    /* We have video already, do nothing */
    if( THEMIM->getIM()->b_has_video )
    {

    }
    else
    {
        /* Show the panel to the user */
        fprintf( stderr, "Showing panel\n" );
    }
}

void BackgroundWidget::resizeEvent( QResizeEvent *e )
{
    if( e->size().height() < ICON_SIZE -1 )
        label->setMaximumWidth( e->size().height() );
    else
        label->setMaximumWidth( ICON_SIZE );
}

/**********************************************************************
 * Playlist Widget. The embedded playlist
 **********************************************************************/
#include "components/playlist/panels.hpp"
#include "components/playlist/selector.hpp"

PlaylistWidget::PlaylistWidget( intf_thread_t *_p_intf ) : QFrame(NULL),
                                                            p_intf( _p_intf )
{
    selector = new PLSelector( this, p_intf, THEPL );
    selector->setMaximumWidth( 130 );

    playlist_item_t *p_root = playlist_GetPreferredNode( THEPL,
                                                THEPL->p_local_category );

    rightPanel = qobject_cast<PLPanel *>(new StandardPLPanel( this,
                              p_intf, THEPL, p_root ) );

    CONNECT( selector, activated( int ), rightPanel, setRoot( int ) );

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget( selector, 0 );
    layout->addWidget( rightPanel, 10 );
    setLayout( layout );
}

PlaylistWidget::~PlaylistWidget()
{
}

QSize PlaylistWidget::sizeHint() const
{
    return widgetSize;
}

