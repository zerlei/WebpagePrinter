{
  description =
    "Print a web page through an http request or a websocket connection, and skip the browser's print preview";

  inputs = { nixpkgs.url = "github:NixOS/nixpkgs/nixos-24.11"; };

  outputs = { self, nixpkgs }:
    let pkgs = nixpkgs.legacyPackages.x86_64-linux;
    in {
      packages.x86_64-linux.default = pkgs.stdenv.mkDerivation {
        pname = "WebpagePrinterHelper";
        version = "0.1";

        # The QtQuick project we created with Qt Creator's project wizard is here
        src = ./.;

        buildInputs = [
          pkgs.kdePackages.qtbase
          pkgs.kdePackages.full
          pkgs.kdePackages.qtwebengine
          pkgs.kdePackages.qthttpserver
        ];

        nativeBuildInputs = [ pkgs.cmake pkgs.kdePackages.wrapQtAppsHook ];

      };

      devShells.x86_64-linux.default = pkgs.mkShell {

        inputsFrom = [ self.packages.x86_64-linux.default ];
        buildInputs = with pkgs; [
          graphviz
          doxygen
          gdb
          # this is for the shellhook portion
          qt6.wrapQtAppsHook
          makeWrapper
          bashInteractive
        ];
        shellHook = ''
          # 不知道为什么这个版本的compiler_command.json 导不出这个目录，手动加上吧！
          cat > .clangd <<EOF
          CompileFlags:
            Add: [-I${pkgs.qt6.qtbase}/include]
          EOF
          # set the environment variables that unpatched Qt apps expect
          bashdir=$(mktemp -d)
          makeWrapper "$(type -p bash)" "$bashdir/bash" "''${qtWrapperArgs[@]}"
          exec "$bashdir/bash"
        '';
      };
    };
}
