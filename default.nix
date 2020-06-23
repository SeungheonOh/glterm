{ nixpkgs ? import <nixpkgs> { }}:
let 
  pkgs = with nixpkgs; [
    pkg-config
    libGL
    glfw3
    libGLU
    gdb
  ];

in 
  nixpkgs.stdenv.mkDerivation{
    name = "term";
    buildInputs = pkgs;
    meta = {
    };
  }
