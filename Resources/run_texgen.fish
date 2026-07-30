#!/usr/bin/env fish

# Adjust this path if your venv folder has a different name/location
set VENV_PATH ./venv

if not test -f $VENV_PATH/bin/activate.fish
    echo "Could not find venv activation script at $VENV_PATH/bin/activate.fish"
    exit 1
end

source $VENV_PATH/bin/activate.fish
python3 TexGen.py
deactivate
