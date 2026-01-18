# moonbar

> A GTK4 based status bar, scripted and configured in lua + styled w/ css

## Installing

### Requirements

In order to install you will need the following installed:

- git
- cmake
- gtk4
- gtk4-layer-shell
- cmake
- ninja
- clang

### Building

#### Build using Task

If you have [task](https://taskfile.dev) installed you can do the following for easier builds:

```sh
task configure -- debug
task
```

#### Building Manually

You can build by doing the following:

```sh
git clone git@github.com:s0cks/moonbar.git && cd moonbar
cmake --preset=debug .
cmake --build build/
```

### Running


#### Running using task

You can run moonbar using [task](https://taskfile.dev/)

```sh
task run
```

#### Running Manually

```sh
# consult w/ bin/run-moonbar
./bin/run-moonba
```

## Example Config

See [example/](example/)

## LICENSE

See [LICENSE](LICENSE)

## Contributing

See [Contributing](Contributing.md)
