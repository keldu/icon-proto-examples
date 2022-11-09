{ pkgs ? import <nixpkgs> {} }:
  pkgs.mkShellNoCC {
    nativeBuildInputs = [ pkgs.gnumake pkgs.gfortran12 pkgs.python39 pkgs.python39Packages.jinja2 ];
  }
