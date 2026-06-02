{
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-25.05";
  inputs.frostix = {
    url = "github:shomykohai/frostix";
    inputs.nixpkgs.follows = "nixpkgs";
  };

  outputs = {
    self,
    nixpkgs,
    ...
  } @ inputs: {
    devShells.x86_64-linux.default = let
      pkgs = import nixpkgs {system = "x86_64-linux";};
      frostix = inputs.frostix.packages.${pkgs.system};
    in
      pkgs.mkShell {
        nativeBuildInputs = [
          pkgs.gcc-arm-embedded
          pkgs.git
          pkgs.python3
          pkgs.gnumake
          pkgs.clang
          pkgs.llvm
          pkgs.lld
          pkgs.cmake
          frostix.gcc-toolchain.gcc-arm-linux-gnueabihf-12
          frostix.gcc-toolchain.gcc-aarch64-linux-gnu
        ];

        shellHook = ''
          export CROSS_COMPILE=arm-none-eabi-
          export CROSS_COMPILE_LINUX=arm-none-linux-gnueabihf-
        '';
      };
  };
}
