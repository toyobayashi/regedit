{
  "variables": {
    "module_path": "./dist",
    "PRODUCT_DIR": "./build/Release"
  },
  'target_defaults': {
    'defines': [
    ]
  },
  'targets': [
    {
      'target_name': 'regedit',
      'sources': [
        'src/index.cpp',
        'src/util.cpp'
      ],
      'includes': [
        './common.gypi'
      ]
    },
    {
      "target_name": "action_after_build",
      "type": "none",
      "dependencies": [ "regedit" ],
      "copies": [
        {
          "files": [ "<(PRODUCT_DIR)/regedit.node" ],
          "destination": "<(module_path)"
        }
      ]
    }
  ]
}
