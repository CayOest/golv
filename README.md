# golv

A game solver library for C++20.

## Dependencies

At the moment, there are two dependencies which can be switched off separately:
* GoogleTest: can be switched off by cmake option `-DENABLE_TESTS`
* Pybind11: can be switched off by cmake option `-DENABLE_PYBIND11`

Both dependencies are built on the fly.

## Build

```
cmake -S . -B build [-DCMAKE_BUILD_TYPE=Release] [-DENABLE_TESTS] [-DENABLE_PYBIND11]
cmake --build build
```

## Run tests

`./build/tests/unit/unit`

## Run examples

`./build/examples/<insert example binary here>`

## Python bindings

When the option `-DENABLE_PYBIND11` is activated, the shared library `libgolv_skat.so` is created in `<BUILD_DIR>/python`.
For fast prototyping, this can be copied to the directory of the python script that wants to use the bindings.
Alternatively, you can set the `PYTHONPATH` environment variable, e. g.:
``` bash
export PYTHONPATH=$PYTHONPATH:/path/to/directory/where/libgolv_skat.so/is/located
```

After that, you need to rename the `libgolv_skat.so` to `golv_skat.so`, because that is the name of the module.
Then write Python code like:
``` Python
import random
import golv_skat

def make_deck():
    deck = []
    for suit in 'cshd':
        for kind in '789JQKTA':
            deck.append(golv_skat.Card(kind+suit))
    return deck

if __name__ == '__main__':
    game = golv_skat.Skat()
    deck = make_deck()
    random.shuffle(deck)

    initial_state = [deck[:10], deck[10:20], deck[20:30], deck[30:32]]
    game.deal(initial_state)
    print(game)

    value, best_move = golv_skat.mws_binary_search(game)
    print(f"Game Value: {value}, Best Move: {best_move}")
```

which generates a random skat game and solves this using the `mws_binary_search` algorithm.
