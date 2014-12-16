isEmpty(QMAKE_QMLPLUGINDUMP) {
    win32:QMAKE_QMLPLUGINDUMP = $$[QT_INSTALL_BINS]\qmlplugindump.exe
    else:QMAKE_QMLPLUGINDUMP = $$[QT_INSTALL_BINS]/qmlplugindump
}

dmpPluginCommand = LD_LIBRARY_PATH=$$OUT_PWD/../build
dmpPluginCommand = $$dmpPluginCommand && $(COPY_FILE) $$_PRO_FILE_PWD_/qmldir $$OUT_PWD/$$DESTDIR/qmldir
dmpPluginCommand = $$dmpPluginCommand && $$QMAKE_QMLPLUGINDUMP $$uri 1.0 $$OUT_PWD/$$DESTDIR/.. > $$OUT_PWD/$$DESTDIR/plugins.qmltypes
dmpPluginCommand = @echo Generating plugin types file... && $$dmpPluginCommand

dmpPluginDeploymentfolders.commands = $$dmpPluginCommand

QMAKE_POST_LINK += $$dmpPluginCommand
