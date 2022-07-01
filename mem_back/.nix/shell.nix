with import <nixpkgs> {};

{ pkgs ? import <nixpkgs> {} }:
  pkgs.mkShell {
    name = "icon-memory-backend-dev";
    nativeBuildInputs = [ gnumake clang_12 clang-tools ];
  }
