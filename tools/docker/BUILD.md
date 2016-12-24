# ${USER}/cross-build
===============

# ビルド方法
以下のコマンドにより、ビルドを行う。

    docker build \
        --no-cache=false \
        --pull \
        --rm=true \
        --tag=${USER}/cross-build .
