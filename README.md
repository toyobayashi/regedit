# regedit

Operating Windows registry using Node.js.

Platform: Windows only

Node Version: `>= 8.11.2 < 9 || >= 10.0.0 < 10.4.0 || >= 10.7.0`

NAPI Version: `>= 3`

``` cmd
> npm install @tybys/regedit
```

## Usage

NOTE: Some API requires running Node.js with administrator privileges.

``` js
const regedit = require('@tybys/regedit')

// Create or open a key
const { result: key } = regedit.createKey(
  regedit.PredefinedKeys.HKEY_LOCAL_MACHINE,
  'SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\MYKEY'
) // Open key if the key exists

// Open a key
const key = regedit.openKey(
  regedit.PredefinedKeys.HKEY_LOCAL_MACHINE,
  'SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\MYKEY'
) // throw error if the key does not exist

// Write key values
regedit.setValue(key, 'MyStringValue', 'string')
regedit.setValue(key, 'MyStringValue2', 'string', regedit.ValueTypes.REG_EXPAND_SZ)
regedit.setValue(key, 'MyDWORDValue', 233)
regedit.setValue(key, 'MyQWORDValue', 233, regedit.ValueTypes.REG_QWORD)
regedit.setValue(key, 'MyQWORDValue2', 4294967296) // QWORD > 4294967295
regedit.setValue(key, 'MyQWORDValue3', 233n) // Node.js >= 10.7.0
regedit.setValue(key, 'MyMultiStringValue', ['foo', 'bar'])
regedit.setValue(key, 'MyBinaryValue', Buffer.from([0]))
regedit.setValue(key, 'MyBinaryValue2', 'string', regedit.ValueTypes.REG_BINARY)

// Read
regedit.queryInfoKey(key)
regedit.queryValue(key, 'MyStringValue') // throw error if the value does not exist
regedit.readKey(key)

// Delete
regedit.deleteValue(key, 'MyStringValue')
regedit.deleteKey(key) // throw error if the key has subkey
regedit.removeKey(key)

// Close
// KeyHandle is closed automaticly when it will be destructed
// It is not neccesary to call closeKey() manually
// regedit.closeKey(key)
```

## API

### Enums

#### `regedit.PredefinedKeys`

[Predefined Keys](https://docs.microsoft.com/en-us/windows/win32/sysinfo/predefined-keys)

``` ts
declare enum PredefinedKeys {
  HKEY_CLASSES_ROOT = -2147483648,
  HKEY_CURRENT_USER = -2147483647,
  HKEY_LOCAL_MACHINE = -2147483646,
  HKEY_USERS = -2147483645,
  HKEY_PERFORMANCE_DATA = -2147483644,
  HKEY_PERFORMANCE_TEXT = -2147483568,
  HKEY_PERFORMANCE_NLSTEXT = -2147483552,
  HKEY_CURRENT_CONFIG = -2147483643,
  HKEY_DYN_DATA = -2147483642,
  HKEY_CURRENT_USER_LOCAL_SETTINGS = -2147483641
}
```

#### `regedit.ValueTypes`

[Registry Value Types](https://docs.microsoft.com/en-us/windows/win32/sysinfo/registry-value-types)

``` ts
declare enum ValueTypes {
  REG_NONE = 0,
  REG_SZ = 1,
  REG_EXPAND_SZ = 2,
  REG_BINARY = 3,
  REG_DWORD = 4,
  REG_DWORD_BIG_ENDIAN = 5,
  REG_LINK = 6,
  REG_MULTI_SZ = 7,
  REG_RESOURCE_LIST = 8,
  REG_FULL_RESOURCE_DESCRIPTOR = 9,
  REG_RESOURCE_REQUIREMENTS_LIST = 10,
  REG_QWORD = 11
}
```

#### `regedit.Disposition`

Returned by [regedit.createKey()](#regedit.createKey).

``` ts
declare enum Disposition {
  /** The key did not exist and was created. */
  REG_CREATED_NEW_KEY = 1,
  /** The key existed and was simply opened without being changed. */
  REG_OPENED_EXISTING_KEY = 2
}
```

### Type alias

#### `regedit.HKEY`

``` ts
declare type HKEY = number;
```

#### `regedit.InputKey`

``` ts
declare type InputKey = HKEY | KeyHandle;
```

* Type alias: [HKEY](#regedit.HKEY)
* Interface: [KeyHandle](#regedit.KeyHandle)

### Interfaces

#### `regedit.KeyHandle`

Returned by [regedit.openKey()](#regedit.openKey).

``` ts
declare interface KeyHandle {
  getValue (): HKEY;
  isClosed (): boolean;
}
```

* Type alias: [HKEY](#regedit.HKEY)

#### `regedit.KeyInfo`

Returned by [regedit.queryInfoKey()](#regedit.queryInfoKey).

``` ts
declare interface KeyInfo {
  className: string;
  subKeys: number;
  maxSubKeyLen: number;
  maxClassLen: number;
  values: number;
  maxValueNameLen: number;
  maxValueLen: number;
  securityDescriptor: number;
  lastWriteTime: Date;
}
```

#### `regedit.ValueInfo`

Returned by [regedit.queryValue()](#regedit.queryValue).

``` ts
declare interface ValueInfo {
  data: number | bigint | string | string[] | Buffer;
  type: ValueTypes;
}
```

* Enum: [ValueTypes](#regedit.ValueTypes)

#### `regedit.ChildKey`

Returned by [regedit.enumKey()](#regedit.enumKey).

``` ts
declare interface ChildKey {
  name: string;
  className: string;
  lastWriteTime: Date;
}
```

#### `regedit.ChildValue`

Returned by [regedit.enumValue()](#regedit.enumValue).

``` ts
declare interface ChildValue extends ValueInfo {
  name: string;
}
```

* Interface: [ValueInfo](#regedit.ValueInfo)

#### `regedit.CreateKeyResult`

Returned by [regedit.createKey()](#regedit.createKey).

``` ts
declare interface CreateKeyResult {
  result: KeyHandle;
  disposition: Disposition;
}
```

* Interface: [KeyHandle](#regedit.KeyHandle)
* Enum: [Disposition](#regedit.Disposition)

#### `regedit.ReadKeyResult`

Returned by [regedit.readKey()](#regedit.readKey).

``` ts
declare interface ReadKeyResult {
  info: KeyInfo;
  keys: ChildKey[];
  values: ChildValue[];
}
```

* Interface: [KeyInfo](#regedit.KeyInfo)
* Interface: [ChildKey](#regedit.ChildKey)
* Interface: [ChildValue](#regedit.ChildValue)

### Functions

#### `regedit.openKey`

[RegOpenKeyExW](https://docs.microsoft.com/en-us/windows/win32/api/winreg/nf-winreg-regopenkeyexw)

``` ts
declare function openKey (key: InputKey, subKey?: string): KeyHandle;
```

* Type alias: [InputKey](#regedit.InputKey)
* Interface: [KeyHandle](#regedit.KeyHandle)

#### `regedit.closeKey`

[RegCloseKey](https://docs.microsoft.com/en-us/windows/win32/api/winreg/nf-winreg-regclosekey)

``` ts
declare function closeKey (key: KeyHandle): void;
```

* Interface: [KeyHandle](#regedit.KeyHandle)

#### `regedit.queryInfoKey`

[RegQueryInfoKeyW](https://docs.microsoft.com/en-us/windows/win32/api/winreg/nf-winreg-regqueryinfokeyw)

``` ts
declare function queryInfoKey (key: InputKey): KeyInfo;
```

* Type alias: [InputKey](#regedit.InputKey)
* Interface: [KeyInfo](#regedit.KeyInfo)

#### `regedit.queryValue`

[RegQueryValueExW](https://docs.microsoft.com/en-us/windows/win32/api/winreg/nf-winreg-regqueryvalueexw)

``` ts
declare function queryValue (key: InputKey, valueName?: string): ValueInfo;
```

* Type alias: [InputKey](#regedit.InputKey)
* Interface: [ValueInfo](#regedit.ValueInfo)

#### `regedit.enumKey`

[RegEnumKeyExW](https://docs.microsoft.com/en-us/windows/win32/api/winreg/nf-winreg-regenumkeyexw)

``` ts
declare function enumKey (key: InputKey, index: number): ChildKey;
```

* Type alias: [InputKey](#regedit.InputKey)
* Interface: [ChildKey](#regedit.ChildKey)

#### `regedit.enumValue`

[RegEnumValueW](https://docs.microsoft.com/en-us/windows/win32/api/winreg/nf-winreg-regenumvaluew)

``` ts
declare function enumValue (key: InputKey, index: number): ChildValue;
```

* Type alias: [InputKey](#regedit.InputKey)
* Interface: [ChildValue](#regedit.ChildValue)

#### `regedit.createKey`

[RegCreateKeyExW](https://docs.microsoft.com/en-us/windows/win32/api/winreg/nf-winreg-regcreatekeyexw)

``` ts
declare function createKey (key: InputKey, subKey: string, className?: string): CreateKeyResult;
```

* Type alias: [InputKey](#regedit.InputKey)
* Interface: [CreateKeyResult](#regedit.CreateKeyResult)

#### `regedit.deleteKey`

[RegDeleteKeyW](https://docs.microsoft.com/en-us/windows/win32/api/winreg/nf-winreg-regdeletekeyw)

``` ts
declare function deleteKey (key: InputKey, subKey?: string): void;
```

* Type alias: [InputKey](#regedit.InputKey)

#### `regedit.setValue`

[RegSetValueExW](https://docs.microsoft.com/en-us/windows/win32/api/winreg/nf-winreg-regsetvalueexw)

``` ts
declare function setValue (key: InputKey, valueName: string, data?: number | bigint | string | string[] | Buffer, type?: ValueTypes): void;
```

* Type alias: [InputKey](#regedit.InputKey)
* Interface: [ValueTypes](#regedit.ValueTypes)

#### `regedit.deleteValue`

[RegDeleteValueW](https://docs.microsoft.com/en-us/windows/win32/api/winreg/nf-winreg-regdeletevaluew)

``` ts
declare function deleteValue (key: InputKey, valueName?: string): void;
```

* Type alias: [InputKey](#regedit.InputKey)

#### `regedit.readKey`

Read a registry key and its child key/values.

``` ts
declare function readKey (key: InputKey, subKey?: string): ReadKeyResult;
```

* Type alias: [InputKey](#regedit.InputKey)
* Interface: [ReadKeyResult](#regedit.ReadKeyResult)

#### `regedit.removeKey`

Remove a registry key recursively.

``` ts
declare function removeKey (key: InputKey, subKey?: string): void;
```

* Type alias: [InputKey](#regedit.InputKey)
