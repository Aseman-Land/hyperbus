isEmpty(QMAKE_QMLPLUGINDUMP) {
    win32:QMAKE_QMLPLUGINDUMP = $$[QT_INSTALL_BINS]/qmlplugindump.exe
    else:QMAKE_QMLPLUGINDUMP = $$[QT_INSTALL_BINS]/qmlplugindump
}

qmldirSrc = $$_PRO_FILE_PWD_/qmldir
qmkdirDst = $$OUT_PWD/$$DESTDIR/qmldir

win32 {
    qmldirSrc = $$replace(qmldirSrc, /, \\)
    qmldirSrc ~= s,\\\\\\.?\\\\,\\,

    qmkdirDst = $$replace(qmkdirDst, /, \\)
    qmkdirDst ~= s,\\\\\\.?\\\\,\\,
}

dmpPluginCommand = $(COPY_FILE) $$qmldirSrc $$qmkdirDst
unix: dmpPluginCommand = $$dmpPluginCommand && $$QMAKE_QMLPLUGINDUMP $$uri 1.0 $$OUT_PWD/$$DESTDIR/.. > $$OUT_PWD/$$DESTDIR/plugins.qmltypes
unix: dmpPluginCommand = LD_LIBRARY_PATH=$$OUT_PWD/../build && $$dmpPluginCommand
dmpPluginCommand = @echo Generating plugin types file... && $$dmpPluginCommand

dmpPluginDeploymentfolders.commands = $$dmpPluginCommand

QMAKE_POST_LINK += $$dmpPluginCommand
