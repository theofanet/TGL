#!/bin/bash
#!/usr/bin/env bash
set -eo pipefail


function gen:project {
    cd ..
    ./vendor/bin/premake/premake5 gmake2
}
