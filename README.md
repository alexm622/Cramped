# Cramped [WIP]

user space limitations implemented using the linux kernel, block devices, and loopback devices

## compatibility

This utility is only available for linux-based systems. Unix support is not being developed and will not be developed until loop devices or similar functionality is added.

## Table of Contents

- [Compiling](#Compiling)
- [Installation](#Installation)
- use
- configuration
- FAQS
- Contributing

## Compiling

### Dependencies: <br>

- `Boost-libs`
- either Systemd or rc.d (both will be officially supported in the end)

## Installation

### !! This is currently WIP !!

### there is currently no packaging of any kind into a release media

### anything listed below is either planned or in the works.

### debian/ubuntu

The idea is to eventually have this available on mainstream distribution repo's but you will also be able to download release debs from github

`apt install Cramped`

It is suggested that you use `gdebi` because it will also install all dependencies that are related to the package.

`gdebi install Cramped-latest.deb`

### Red hat

using yum

`yum install Cramped`

using rpm

`rpm install Cramped-latest.deb`

### arch

probably going to need to use AUR

this is still in the works atm

### from source

1. download the source code from the github releases page, or clone the repo with `git clone https://github.com/alexm622/cramped`
2. see [Compiling](#Compiling) for compilation instructions
3. use `make install` to install the package and `make uninstall` to uninstall the package
