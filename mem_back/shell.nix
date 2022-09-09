{ pkgs ? import <nixpkgs> {} }:
  pkgs.mkShell {
    nativeBuildInputs = [ pkgs.gnumake pkgs.gcc pkgs.python3 ];
  }
