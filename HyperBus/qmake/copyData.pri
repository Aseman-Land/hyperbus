
defineTest(copyData) {
for(deploymentfolder, COPYFOLDERS) {
    item = item$${deploymentfolder}
    itemsources = $${item}.sources
    $$itemsources = $$eval($${deploymentfolder}.source)
    itempath = $${item}.path
    $$itempath= $$eval($${deploymentfolder}.target)
    export($$itemsources)
    export($$itempath)
    DEPLOYMENT += $$item
}

COPY_MAINPROFILEPWD = $$PWD

isEmpty(ASEMAN_BUILD_DEST): DEST_OUT_PWD = $$OUT_PWD/

copyCommand =
for(deploymentfolder, COPYFOLDERS) {
for(sourceFiles, $${deploymentfolder}.source) {
    sourceFiles = $$relative_path($$sourceFiles, $$COPY_MAINPROFILEPWD)
    source = $$COPY_MAINPROFILEPWD/$$sourceFiles
    source = $$replace(source, /, \\)
    sourcePathSegments = $$split(source, \\)
    !isEqual(source,$$target) {
        !isEmpty(copyCommand):copyCommand += &&
        win32 {
            target = $$DEST_OUT_PWD$$eval($${deploymentfolder}.target)/$$last(sourcePathSegments)
            target = $$replace(target, /, \\)
            target ~= s,\\\\\\.?\\\\,\\,
            copyCommand += cmd /c echo F | xcopy /E /Y /I \"$$source\" \"$$target\"
        } else {
            source = $$replace(source, \\\\, /)
            target = $$DEST_OUT_PWD$$eval($${deploymentfolder}.target)/
            target = $$replace(target, \\\\, /)
            copyCommand += test -d \"$$target\" || mkdir -p \"$$target\" && cp -f -u -r \"$$source\" \"$$target\"
        }
    }
}
}
!isEmpty(copyCommand) {
    copyCommand = @echo Copying application data... && $$copyCommand
    copydeploymentfolders.commands = $$copyCommand
    first.depends = $(first) copydeploymentfolders
    export(first.depends)
    export(copydeploymentfolders.commands)
    QMAKE_EXTRA_TARGETS += first copydeploymentfolders
}

export (DEPLOYMENT)
export (QMAKE_EXTRA_TARGETS)
}
