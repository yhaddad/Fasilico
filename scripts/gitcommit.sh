#!/usr/bin/sh

function gco {
    echo "=== git commit ==="
    
    git init
    echo "--->> HistBook"
    git add HistBook/src/*.cc
    git add HistBook/include/*.h
    echo "--->> JetFinder"
    git add JetFinder/src/*.cc
    git add JetFinder/include/*.h
    echo "--->> FastSim"
    git add FastSim/src/*.cc
    git add FastSim/include/*.h

    echo "--->> script"
    git add scripts/*
    
    echo "--->> cmake"
    git add cmake/*.cmake
    
    git commit -m "commit for :: $1"
}

function gpush {
    echo "=== git push ==="
    
    git remote add origin git@github.com:yhaddad/Fasilico.git
    git push -u origin master
    
}
