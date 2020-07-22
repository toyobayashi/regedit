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
        'src/value_types.cpp',
        'src/predefined_keys.cpp',
        'src/disposition.cpp',
        'src/open_key.cpp',
        'src/close_key.cpp',
        'src/query_info_key.cpp',
        'src/query_value.cpp',
        'src/enum_key.cpp',
        'src/enum_value.cpp',
        'src/create_key.cpp',
        'src/delete_key.cpp',
        'src/set_value.cpp',
        'src/delete_value.cpp',
        'src/KeyHandle.cpp',
        'src/value.cpp',
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
