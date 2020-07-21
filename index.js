if (process.platform !== 'win32') {
  throw new Error('This package only support Windows platform.')
}

const addon = require('./dist/regedit.node')

function makeEnum (obj) {
  Object.keys(obj).forEach((k) => {
    if (!Object.prototype.hasOwnProperty.call(obj, obj[k])) {
      obj[obj[k]] = k
    }
  })
}

makeEnum(addon.PredefinedKeys)
makeEnum(addon.ValueTypes)

module.exports = addon
