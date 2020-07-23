if (process.platform !== 'win32') {
  throw new Error('This package only support Windows platform.')
}

const path = require('path')
const addon = require('./dist/regedit.node')

Object.keys(addon).forEach(k => {
  exports[k] = addon[k]
})

exports.readKey = readKey
exports.removeKey = removeKey

function readKey (key, subKey) {
  const hKey = addon.openKey(key, subKey)
  const info = addon.queryInfoKey(hKey)
  const keys = []
  for (let i = 0; i < info.subKeys; i++) {
    keys.push(addon.enumKey(hKey, i))
  }
  const values = []
  for (let i = 0; i < info.values; i++) {
    values.push(addon.enumValue(hKey, i))
  }
  return { info, keys, values }
}

function removeKey (key, subKey) {
  const { keys } = readKey(key, subKey)
  for (let i = 0; i < keys.length; i++) {
    removeKey(key, path.win32.join(subKey, keys[i].name))
  }
  addon.deleteKey(key, subKey)
}
