include $(top_srcdir)/modules/common.am

guidir = $(pluginsdir)/gui


libmacosx_plugin_la_OBJCFLAGS = $(AM_OBJCFLAGS) -fobjc-exceptions -fobjc-arc -F${CONTRIB_DIR}
libmacosx_plugin_la_LDFLAGS = $(AM_LDFLAGS) -rpath '$(guidir)' \
	-Wl,-framework,Cocoa -Wl,-framework,Carbon -Wl,-framework,CoreServices \
	-Wl,-framework,AVFoundation -Wl,-framework,CoreMedia -Wl,-framework,IOKit \
	-Wl,-framework,AddressBook -Wl,-framework,WebKit -Wl,-framework,CoreAudio \
	-Wl,-framework,SystemConfiguration -Wl,-framework,ScriptingBridge \
	-F${CONTRIB_DIR} -Wl,-framework,BGHUDAppKit -Wl,-framework,QuartzCore

if HAVE_SPARKLE
libmacosx_plugin_la_LDFLAGS += -Wl,-framework,Sparkle
endif

EXTRA_LTLIBRARIES = libmacosx_plugin.la
gui_LTLIBRARIES = $(LTLIBmacosx)

libmacosx_plugin_la_SOURCES = \
	helpers.h \
	VLCAboutWindowController.h VLCAboutWindowController.m \
	VLCAddonListItem.h VLCAddonListItem.m \
	VLCAddonsWindowController.h VLCAddonsWindowController.m \
	applescript.h applescript.m \
	AppleRemote.h AppleRemote.m \
	BWQuincyManager.h BWQuincyManager.m \
	BWQuincyUI.h BWQuincyUI.m \
	CompatibilityFixes.h \
	VLCControlsBarCommon.m VLCControlsBarCommon.h \
	VLCDocumentController.m VLCDocumentController.h \
	VLCMainWindowControlsBar.m VLCMainWindowControlsBar.h \
	VLCConvertAndSaveWindowController.h VLCConvertAndSaveWindowController.m \
	VLCCoreInteraction.h VLCCoreInteraction.m \
	VLCExtensionsDialogProvider.h VLCExtensionsDialogProvider.m \
	VLCExtensionsManager.h VLCExtensionsManager.m \
	VLCHelpWindowController.h VLCHelpWindowController.m \
	VLCMain+OldPrefs.h VLCMain+OldPrefs.m \
	VLCMain.h VLCMain.m \
	VLCApplication.h VLCApplication.m \
	iTunes.h \
	VLCKeyboardBacklightControl.h VLCKeyboardBacklightControl.m \
	macosx.m \
	VLCMainMenu.h VLCMainMenu.m \
	VLCMainWindowTitleView.h VLCMainWindowTitleView.m \
	misc.h misc.m \
	VLCOpenWindowController.h VLCOpenWindowController.m \
	VLCOutput.h VLCOutput.m \
	VLCPlaylistView.h VLCPlaylistView.m \
	VLCPlaylist.h VLCPlaylist.m \
	VLCPlaylistInfo.h VLCPlaylistInfo.m \
	VLCPLItem.h VLCPLItem.m \
	VLCPLModel.h VLCPLModel.m \
	prefs.h prefs.m \
	prefs_widgets.h prefs_widgets.m \
	PXSourceList.h PXSourceList.m \
	PXSourceListDataSource.h PXSourceListDelegate.h \
	SideBarItem.h SideBarItem.m \
	VLCPopupPanelController.h VLCPopupPanelController.m \
	VLCTextfieldPanelController.h VLCTextfieldPanelController.m \
	simple_prefs.h simple_prefs.m \
	SPInvocationGrabbing.h SPInvocationGrabbing.m \
	SPMediaKeyTap.h SPMediaKeyTap.m \
	Spotify.h \
	VLCStringUtility.h VLCStringUtility.m \
	VLCVoutView.h VLCVoutView.m \
	VLCTimeSelectionPanelController.h VLCTimeSelectionPanelController.m \
	VLCUIWidgets.h VLCUIWidgets.m \
	VLCScrollingClipView.h VLCScrollingClipView.m \
	VLCStatusBarIcon.h VLCStatusBarIcon.m \
	VLCVoutWindowController.h VLCVoutWindowController.m \
	Windows.h Windows.m \
	VLCAudioEffectsWindowController.h VLCAudioEffectsWindowController.m \
	VLCBookmarksWindowController.h VLCBookmarksWindowController.m \
	VLCCoreDialogProvider.h VLCCoreDialogProvider.m \
	VLCDebugMessageWindowController.h VLCDebugMessageWindowController.m \
	VLCHUDSliderCell.h VLCHUDSliderCell.m \
	VLCHUDButtonCell.h VLCHUDButtonCell.m \
	VLCHUDCheckboxCell.h VLCHUDCheckboxCell.m \
	VLCHUDRadiobuttonCell.h VLCHUDRadiobuttonCell.m \
	VLCHUDTextFieldCell.h VLCHUDTextFieldCell.m \
	VLCInputManager.h VLCInputManager.m \
	VLCMainWindow.h VLCMainWindow.m \
	VLCRendererDiscovery.h VLCRendererDiscovery.m \
	VLCRendererItem.h VLCRendererItem.m \
	VLCRendererDialog.h VLCRendererDialog.m \
	VLCResumeDialogController.h VLCResumeDialogController.m \
	VLCTrackSynchronizationWindowController.h VLCTrackSynchronizationWindowController.m \
	VLCVideoEffectsWindowController.h VLCVideoEffectsWindowController.m \
	VLCFSPanelController.h VLCFSPanelController.m
