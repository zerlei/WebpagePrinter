{
  description =
    "Print a web page through an http request or a websocket connection, and skip the browser's print preview";

  inputs = { nixpkgs.url = "github:NixOS/nixpkgs/nixos-25.05"; };

  outputs = { self, nixpkgs }:
    let
      pkgs = nixpkgs.legacyPackages.x86_64-linux;
      PLANTUML_JAR = pkgs.fetchurl {
        url =
          "https://github.com/plantuml/plantuml/releases/download/v1.2025.2/plantuml.jar";
        sha256 =
          "sha256-hix9bQ073jyBnqxN/APPVJBGvx1JwE0Yp3nrLYNLd8k="; # 修改为实际 hash
      };
    in {
      packages.x86_64-linux.default = pkgs.stdenv.mkDerivation {
        pname = "WebpagePrinterTool";
        version = "0.1";

        src = ./.;

        buildInputs = [
          pkgs.kdePackages.qtbase
          pkgs.kdePackages.full
          pkgs.kdePackages.qtwebengine
          pkgs.kdePackages.qthttpserver
          pkgs.kdePackages.qtwebsockets
        ];

        nativeBuildInputs = [ pkgs.cmake pkgs.kdePackages.wrapQtAppsHook ];
        configurePhase = ''
          cmake --preset nixbuild
        '';
        buildPhase = ''
          cmake --build build
        '';
        installPhase = ''
          mkdir -p $out/bin
          cp build/src/WebpagePrinterTool $out/bin/
        '';
      };

      devShells.x86_64-linux.default = pkgs.mkShell {

        inputsFrom = [ self.packages.x86_64-linux.default ];
        buildInputs = [
          # for plantuml 500mb+500mb ！！！！ java...
          pkgs.openjdk

          pkgs.graphviz
          pkgs.doxygen
          pkgs.gdb
          # this is for the shellhook portion
          pkgs.qt6.wrapQtAppsHook
          pkgs.makeWrapper
          pkgs.bashInteractive
        ];
        shellHook = ''
          # set the environment variables that unpatched Qt apps expect
          bashdir=$(mktemp -d)
          makeWrapper "$(type -p bash)" "$bashdir/bash" "''${qtWrapperArgs[@]}"
          # exec "$bashdir/bash"

          # export plantuml jar path
          export PLANTUML_JAR_PATH=${PLANTUML_JAR}
          # 不知道为什么这个版本QT的compiler_command.json 导不出这个目录，手动加上吧！
          export QT6_BASE_PATH=${pkgs.kdePackages.qtbase}
          export QT6_HTTPSERVER_PATH=${pkgs.kdePackages.qthttpserver}
          export QT6_WEBSOCKET_PATH=${pkgs.kdePackages.qtwebsockets}
        '';
      };
    };
}
