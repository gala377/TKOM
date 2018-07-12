## TKOM
### Simple language for concurrent programming.
This project includes translator program (written in C++17) translating simple language "Conc" to Golang.

## Do we need another language?
No, we don't. This project was made for uni classes about compilation techniques.
But I had fun doing it so I had an urge to share it.

## What does a repository Include?
- Sources of translator program translating Conc dialect to Golang.
- Shell script taking care of logging files and cleanup after translation.
- Some code examples.
- **Syntax highlight for visual studio code**

## Conc syntax
Conc is a very simple langugae so you won't get lost.
I'll just show you some code
```
# inline comments are supported

# as well as functions
fn hello() {
  log("Hello") # log function for simple print
}

fn world_now() {
  # log function supports Golang's Printf format (it's actually fmt.Printf)
  log("World!: Now is %v", ___from_time_Now()) # you can call Go's library functions with this
}

fn night_night() {
  log("Waiting for bed")
  # cricital sections in functions guareded by mutexes
  critical {
    log("Good night")
    ___from_time_Sleep(1000)
    log("Good morning")
  }
}

# of course you can take parameters
fn add(a, b) {
  return a + b # and return as well
}

# main as in C
fn main() {
  world_now() # it should be obvious but you can call your functions
  ___parse_raw("var i int64") # want to insert Golang code, sure thing go on! It's unsafe though.
  i = ___from_rand_Int63n(3)
  # want some ifelseifelseifelseifelseif? We have those!
  if i == 0 {
    log("i is 1. I don't care!")
  } else if i == 1 {
    # See this juicy concurrent blocks!
    # every single instruction in those is run in different gorutine. That's where critical sections are needed
    concurrent {
      i = i*(2 + 1) # just adds 2 to i - parethesis supported
      # i = i * 2 - don't do it, otherwise there will be a data race
      {
        night_night()
        hello()
      } # blocks are treated as sigle instruction
      night_night()
      night_night() # night_night's will wait before other ends because there is critical in them
    }
    # concurrent block in itself ends when all instructions in it end
    # so it's not wise to put infinite loops in them
    # don't treat them like threads
    world_now() # check time after 
  } else {
    # simple while loop as well
    loop i > 0 {
      i = i - 1 
    }
  }
}
```

## Installation
You'll need C++17 as well as Go 1.10 and go tool to get a binary.
Translator contains CmakeList so you can just make or cmake in build directory to have outsource build.

## Usage
Copy conc_to_go binary and run.sh from bin folder to the same directory
Then just run run.sh
```
./run.sh input_file.conc output_binary_name
```
If something goes wrong there is logs folder waiting for you full of errors :D 
