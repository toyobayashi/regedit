if (process.platform !== 'win32') {
  throw new Error('This package only support Windows platform.')
}

module.exports = require('./dist/regedit.node')
