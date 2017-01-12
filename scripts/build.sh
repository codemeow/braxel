#!/bin/bash

# «Braxel» - Braille-based console drawer
#
#  Copyright (C) Alexey Shishkin 2016-2017
#
#  This file is part of Project Braxel.
#
#  Project Braxel is free software: you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  Project Braxel is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public License
#  along with Project Braxel. If not, see <http://www.gnu.org/licenses/>.

# Run me as `build.sh release` or `build.h debug` from the project root directory

GLOBAL_PROJECT_NAME=braxel
GLOBAL_DEBUG_MODE=$1

FILE_FILELIST="$GLOBAL_PROJECT_NAME.files"
FILE_CONFIG="$GLOBAL_PROJECT_NAME.config"
FILE_INCLUDES="$GLOBAL_PROJECT_NAME.includes"
FILE_SUBPROJECTS="$GLOBAL_PROJECT_NAME.subprojects"

# Functions section
Die()
{
    echo "$@" 1>&2
    exit 1
}

Initialise()
{
    if [ "$GLOBAL_DEBUG_MODE" == "debug" ]
    then
        COMPILER_DEBUG="-g"
    elif [ "$GLOBAL_DEBUG_MODE" == "release" ]
    then
        COMPILER_DEBUG=""
    else
        Die "Wrong debug type, give 'debug' or 'release'"
    fi

    mkdir -p ./bin
    mkdir -p ./build
}

Include()
{
    echo "┌─── Including"
    echo "├ ./$FILE_CONFIG"
    source "./$FILE_CONFIG"

    FILE_EXPORT="$DIRECTORY_SCRIPTS/export.sh"

    echo "├ ./$FILE_EXPORT"
    source "./$FILE_EXPORT"
    echo "└───"
}

CleanSingle()
{
    DIRECTORY_CLEAN=$1
    find "./$DIRECTORY_CLEAN" ! -path . ! -path "./$DIRECTORY_CLEAN" | while read -r file ;
    do
       echo "├ $file"
       rm -rf $file
    done
}

CleanBinary()
{
    echo "┌─── Cleaning binary"
    CleanSingle $DIRECTORY_BIN
    echo "└───"
}

CleanObject()
{
    echo "┌─── Cleaning objects"
    CleanSingle $DIRECTORY_BUILD
    echo "└───"
}

CleanIncludes()
{
    echo "┌─── Cleaning includes"
    CleanSingle $DIRECTORY_INCLUDES
    echo "└───"
}

Configure()
{
    PROJECT_NAME=""
    PROJECT_TYPE=""

    PROJECT_VERSION_MAJOR=""
    PROJECT_VERSION_MINOR=""
    PROJECT_VERSION_BUILD=""

    COMPILER_NAME=""
    COMPILER_STD=""
    COMPILER_OPTIONS=""

    DEPENDENCY_LINE=""
    DEPENDENCY_PATH=""
    DEPENDENCY_CINCLUDES=""

    LINKER_OPTIONS=""

    EXPORT_FILE_PREFIX=""

    DIRECTORY_DESTINATION="."

    DIRECTORY_PROJECT=$1
    source $DIRECTORY_PROJECT/$SUBPROJECT_FILE_CONFIG

    if [ "$PROJECT_TYPE" == "lib" ]
    then
        COMPILER_OPTIONS+=" -fPIC"
        LINKER_OPTIONS+=" -shared"
        OUTPUT_EXTENSION=".so"
        PROJECT_BINARY="lib$PROJECT_NAME"

        VERSION_M=".$PROJECT_VERSION_MAJOR"
        VERSION_MM=$VERSION_M.$PROJECT_VERSION_MINOR
        VERSION_MMB=$VERSION_MM.$PROJECT_VERSION_BUILD
    elif [ "$PROJECT_TYPE" == "app" ]
    then
        OUTPUT_EXTENSION=""
        PROJECT_BINARY="$PROJECT_NAME"

        VERSION_M=""
        VERSION_MM=""
        VERSION_MMB=""
    else
        Die "Wrong PROJECT_TYPE"
    fi
}

IncrementBuild()
{
    if [ ! -z $PROJECT_VERSION_BUILD ]
    then
        BUILD_NUMBER_CURRENT=$(grep '^PROJECT_VERSION_BUILD' \
                              --color=never \
                              ./$DIRECTORY_PROJECT/$SUBPROJECT_FILE_CONFIG | \
                              cut -f2- -d=)
        BUILD_NUMBER_NEXT=$BUILD_NUMBER_CURRENT
        ((BUILD_NUMBER_NEXT++))
        SED_BEFORE="PROJECT_VERSION_BUILD=$BUILD_NUMBER_CURRENT"
        SED_AFTER="PROJECT_VERSION_BUILD=$BUILD_NUMBER_NEXT"
        sed -i "/^$SED_BEFORE/c\\$SED_AFTER" \
            ./$DIRECTORY_PROJECT/$SUBPROJECT_FILE_CONFIG
    fi
}

Compile()
{
    echo "┌─── Compiling"
    find $DIRECTORY_PROJECT -name "*.c" | while read -r file_c ;
    do
        echo "├ $file_c"
        file_o=$(basename "${file_c%.*}.o")
        $COMPILER_NAME \
            $COMPILER_OPTIONS \
            $COMPILER_DEBUG \
            $DEPENDENCY_INCLUDES \
            -I$DIRECTORY_INCLUDES \
            -std=$COMPILER_STD \
            -c $file_c \
            -o "./$DIRECTORY_BUILD/$file_o"
    done
    echo "└───"
}

Link()
{
    echo "┌─── Linking"
    find ./$DIRECTORY_BUILD -name "*.o" | while read -r file_o ;
    do
        echo "├ $file_o"
    done

    $COMPILER_NAME $LINKER_OPTIONS \
       -o "./$DIRECTORY_BIN/$PROJECT_BINARY$OUTPUT_EXTENSION$VERSION_MMB" \
       ./$DIRECTORY_BUILD/*.o \
       $DEPENDENCY_PATH $DEPENDENCY_LIST
    echo "└───"
}

Symlink()
{
    echo "┌─── Symlinking"
    if [ "$DIRECTORY_DESTINATION" != "." ]
    then
        mkdir -p "./$DIRECTORY_BIN/$DIRECTORY_DESTINATION"
        mv "./$DIRECTORY_BIN/$PROJECT_BINARY$OUTPUT_EXTENSION$VERSION_MMB" \
           "./$DIRECTORY_BIN/$DIRECTORY_DESTINATION/$PROJECT_BINARY$OUTPUT_EXTENSION$VERSION_MMB"
    fi
    if [ ! -z "$VERSION_MMB" ]
    then
        echo  "├ ./$DIRECTORY_BIN/$DIRECTORY_DESTINATION/$PROJECT_BINARY$OUTPUT_EXTENSION$VERSION_MM"
        ln -s "$PROJECT_BINARY$OUTPUT_EXTENSION$VERSION_MMB" \
              "./$DIRECTORY_BIN/$DIRECTORY_DESTINATION/$PROJECT_BINARY$OUTPUT_EXTENSION$VERSION_MM"

        echo  "├ ./$DIRECTORY_BIN/$DIRECTORY_DESTINATION/$PROJECT_BINARY$OUTPUT_EXTENSION$VERSION_M"
        ln -s "$PROJECT_BINARY$OUTPUT_EXTENSION$VERSION_MMB" \
              "./$DIRECTORY_BIN/$DIRECTORY_DESTINATION/$PROJECT_BINARY$OUTPUT_EXTENSION$VERSION_M"

        echo  "├ ./$DIRECTORY_BIN/$DIRECTORY_DESTINATION/$PROJECT_BINARY$OUTPUT_EXTENSION"
        ln -s "$PROJECT_BINARY$OUTPUT_EXTENSION$VERSION_MMB" \
              "./$DIRECTORY_BIN/$DIRECTORY_DESTINATION/$PROJECT_BINARY$OUTPUT_EXTENSION"
    fi
    echo "└───"
}

Export()
{
    echo "┌─── Exporting"
    if [ "$PROJECT_TYPE" == "lib" ]
    then
        ExportSingle
    fi
    echo "└───"
}


# Run section
Initialise
Include
CleanBinary
CleanIncludes
grep -v '^$\|^\s*\#' $FILE_SUBPROJECTS | while read -r subproject ;
do
    CleanObject
    Configure "./$DIRECTORY_CODE/$subproject"
    IncrementBuild
    Compile
    Link
    Symlink
    Export
done
CleanObject
