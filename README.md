# Stack

## Description

This is my stack implementation. It's different from common stack implementation because of different protection types:
- Hash protection
- Canary protection

This stack use abstract type declared in Types.h

## Install 

```
git clone https://github.com/d3clane/Stack.git
make buildDirs
make
```

## Hash protection

Program calculates hash of all data in stack. There are two hashes:
- Hashing stack data
- Hashing stack structure (size, data adr and etc.)

Program calculates hash on each func call and compares it with the previous one saved. If they are not the same functions return error and don't do anything.

## Canary protection

Canaries in my case are "hex speak" values that are set before and after some data:
- before and after stack structure with info
- before and after stack data

On each function call program compares canary values with values that have to be there. If they are not the same function return error and don't do anything.

## Usage

There could be different bugs in program. For example, some data could go beyond the array bonders and ruin my data. These types of protection will find out these data corruptions and stop program executing. Also it may be useful when program have to be super safe and I don't want anyone to use buffer overflow vulnerability if they find one of them
