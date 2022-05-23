with import <nixpkgs> {};

stdenv.mkDerivation {
  name = "icon-memory-backend-dev";
  buildInputs = [ gnumake clang_12 clang-tools ];

  buildPhase = ''
  '';
}
