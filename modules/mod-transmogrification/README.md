# Transmog Module

- Latest Transmog build status with warheadcore: [![Build Status](https://travis-ci.org/warheadcore/mod-transmog.svg?branch=master)](https://travis-ci.org/warheadcore/mod-transmog)

This is a module for [warheadCore](http://www.warheadcore.org) that adds transmog feature, it's based on [Rochet2 Transmog Script](http://rochet2.github.io/Transmogrification.html) 

## Important notes

You have to use at least this warheadCore commit:

<https://github.com/warheadcore/warheadcore-wotlk/commit/b6cb9247ba96a862ee274c0765004e6d2e66e9e4>

If using this module with an warheadCore commit older than

<https://github.com/warheadcore/warheadcore-wotlk/commit/b34bc28e5b02514fca3519beac420c58faa89cad>

please delete the IDs 50000 and 50001 from npc_text before upgrading warheadCore:
```sql
DELETE FROM `npc_text` WHERE `ID` IN (50000,50001);
```
Otherwise there will be conflicts for these IDs. The module will now use IDs 601083 and 601084 as default.

## Requirements

Transmogrification module currently requires:

warheadCore v1.0.2+

## How to install

### 1) Simply place the module under the `modules` folder of your warheadCore source folder.

You can do clone it via git under the warheadcore/modules directory:

```sh
cd path/to/warheadcore/modules
git clone https://github.com/warheadcore/mod-transmog.git
```

or you can manually [download the module](https://github.com/warheadcore/mod-transmog/archive/master.zip), unzip the Transmog folder and place it under the `warheadcore/modules` directory.

### 2) Re-run cmake and launch a clean build of warheadCore

**That's it.**

### (Optional) Edit module configuration

If you need to change the module configuration, go to your server configuration folder (e.g. **etc**), copy `transmog.conf.dist` to `transmog.conf` and edit it as you prefer.


## License

This module is released under the [GNU AGPL license](https://github.com/warheadcore/mod-transmog/blob/master/LICENSE).





