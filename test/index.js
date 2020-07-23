const addon = require('..')
console.log(addon)

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
  console.log(hKey.getValue())
  const info = addon.queryInfoKey(hKey)
  const r = []
  for (let i = 0; i < info.subKeys; i++) {
    r.push(addon.enumKey(hKey, i))
  }
  console.log(hKey.isClosed())
  addon.closeKey(hKey)
  console.log(hKey.isClosed())
  addon.closeKey(hKey)
  addon.closeKey(hKey)
  addon.closeKey(hKey)
  return r
}



function testEnumValue () {
  const hKey = addon.openKey(addon.PredefinedKeys.HKEY_LOCAL_MACHINE, 'SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Git_is1')
  const info = addon.queryInfoKey(hKey)
  console.log(info)
  const r = []
  for (let i = 0; i < info.values; i++) {
    r.push(addon.enumValue(hKey, i))
  }
  addon.closeKey(hKey)
  return r
}

function testCreateKey () {
  // const { result, disposition } = addon.createKey(addon.PredefinedKeys.HKEY_LOCAL_MACHINE, 'SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\MYTEST')
  // addon.createKey(addon.PredefinedKeys.HKEY_LOCAL_MACHINE, 'SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\MYTEST\\a\\b\\c')
  // console.log(result.getValue())
  // console.log(addon.Disposition[disposition])

  // addon.setValue(result, '字符串', '中文')
  // addon.setValue(result, '字符串2', '中文', addon.ValueTypes.REG_BINARY)
  // addon.setValue(result, '多字符串', ['中文', 'english', 'にほんご'])
  // addon.setValue(result, '可扩展字符串', '中文2', addon.ValueTypes.REG_EXPAND_SZ)
  // addon.setValue(result, '链接', 'D:\\app\\nodev\\nodev.exe', addon.ValueTypes.REG_LINK)
  // addon.setValue(result, '32位数值', 100)
  // addon.setValue(result, '32位数值大端', 100, addon.ValueTypes.REG_DWORD_BIG_ENDIAN)
  // addon.setValue(result, '64位数值', 100, addon.ValueTypes.REG_QWORD)
  // if (process.version >= '10.7.0') {
  //   addon.setValue(result, '64位数值BigInt', 100n, addon.ValueTypes.REG_QWORD)
  //   addon.setValue(result, '64位数值BigInt2', 200n)
  // }
  // addon.setValue(result, '64位数值BigInt3', -1, addon.ValueTypes.REG_QWORD)
  // addon.setValue(result, '64位数值BigInt4', 4294967296, addon.ValueTypes.REG_DWORD)
  // addon.deleteKey(result)
  // addon.closeKey(result)
  // addon.closeKey(result)
  addon.removeKey(addon.PredefinedKeys.HKEY_LOCAL_MACHINE, 'SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\MYTEST')
}

// console.log(testEnumKey())
// console.log(addon.readKey(addon.PredefinedKeys.HKEY_LOCAL_MACHINE, 'SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Git_is1'))
testCreateKey()
