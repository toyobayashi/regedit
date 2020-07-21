/// <reference types="node" />

declare namespace regedit {
  export enum PredefinedKeys {
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

  export enum ValueTypes {
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

  type HKEY = number;

  export interface KeyInfo {
    className: string;
    classLength: number;
    subKeys: number;
    maxSubKeyLen: number;
    maxClassLen: number;
    values: number;
    maxValueNameLen: number;
    maxValueLen: number;
    securityDescriptor: number;
    lastWriteTime: Date;
  }

  export interface ValueInfo {
    data: number | bigint | string | string[] | Buffer;
    type: ValueTypes;
  }

  export function openKey (key: HKEY, subKey?: string): HKEY;
  export function closeKey (key: HKEY): void;
  export function queryInfoKey (key: HKEY): KeyInfo;
  export function queryValue (key: HKEY, valueName?: string): ValueInfo;
}

export = regedit; 
