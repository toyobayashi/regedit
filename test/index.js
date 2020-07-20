const addon = require('..')

const hKey = addon.openKey(addon.predefinedKeys.HKEY_LOCAL_MACHINE, 'SOFTWARE\\Microsoft\\Windows\\CurrentVersion')
const r = addon.queryInfoKey(hKey)
console.log(hKey)
console.log(r)
console.log(r.lastWriteTime.toLocaleString())
addon.closeKey(hKey)
