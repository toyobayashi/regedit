const addon = require('..')

/* const hKey = addon.openKey(addon.PredefinedKeys.HKEY_LOCAL_MACHINE, 'SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\{3B98BA21-4079-464C-B23C-E5E19D10EADB}')
const r = addon.queryInfoKey(hKey)
console.log(hKey)
console.log(r)
console.log(r.lastWriteTime.toLocaleString())

console.log(addon.queryValue(hKey, 'DisplayVersion').data)
console.log(addon.queryValue(hKey, 'UninstallString').data)
console.log(addon.queryValue(hKey, 'Language').data)
console.log(addon.queryValue(hKey, 'Size').data)
// console.log(addon.queryValue(hKey, 'T').data)
// console.log(addon.queryValue(hKey, 'S').data)
// console.log(addon.queryValue(hKey, 'M').data)
addon.closeKey(hKey) */

function testEnumKey () {
  const hKey = addon.openKey(addon.PredefinedKeys.HKEY_LOCAL_MACHINE, 'SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall')
  const info = addon.queryInfoKey(hKey)
  const r = []
  for (let i = 0; i < info.subKeys; i++) {
    r.push(addon.enumKey(hKey, i))
  }
  addon.closeKey(hKey)
  return r
}

console.log(testEnumKey())
