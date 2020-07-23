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

  export type HKEY = number;

  export interface KeyInfo {
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

  export interface ValueInfo {
    data: number | bigint | string | string[] | Buffer;
    type: ValueTypes;
  }

  export interface ChildKey {
    name: string;
    className: string;
    lastWriteTime: Date;
  }

  export interface ChildValue extends ValueInfo {
    name: string;
  }

  export interface KeyHandle {
    getValue (): HKEY;
    isClosed (): boolean;
  }

  export type InputKey = HKEY | KeyHandle;

  export enum Disposition {
    REG_CREATED_NEW_KEY = 1,
    REG_OPENED_EXISTING_KEY = 2
  }

  export interface CreateKeyResult {
    result: KeyHandle;
    disposition: Disposition;
  }

  export function openKey (key: InputKey, subKey?: string): KeyHandle;
  export function closeKey (key: KeyHandle): void;
  export function queryInfoKey (key: InputKey): KeyInfo;
  export function queryValue (key: InputKey, valueName?: string): ValueInfo;
  export function enumKey (key: InputKey, index: number): ChildKey;
  export function enumValue (key: InputKey, index: number): ChildValue;
  export function createKey (key: InputKey, subKey: string, className?: string): CreateKeyResult;
  export function deleteKey (key: InputKey, subKey?: string): void;
  export function setValue (key: InputKey, valueName: string, data?: number | bigint | string | string[] | Buffer, type?: ValueTypes): void;
  export function deleteValue (key: InputKey, valueName?: string): void;
  export function readKey (key: InputKey, subKey?: string): { keys: ChildKey[]; values: ChildValue[] };
  export function removeKey (key: InputKey, subKey?: string): void;
}

export = regedit;
