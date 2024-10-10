// Copyright (c) Microsoft Corporation, Nadav Bar, and Felix Rieseberg
// All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the ""License""); you may
// not use this file except in compliance with the License. You may obtain a
// copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// THIS CODE IS PROVIDED ON AN  *AS IS* BASIS, WITHOUT WARRANTIES OR CONDITIONS
// OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION ANY
// IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS FOR A PARTICULAR PURPOSE,
// MERCHANTABLITY OR NON-INFRINGEMENT.
//
// See the Apache Version 2.0 License for specific language governing permissions
// and limitations under the License.

const npmScope = 'nodert-win11-22h2';

function getWindowsMajorVersion() {
  try {
    const os = require('os');
    const version = os.release();
    return parseInt(version.split('.')[0], 10);
  } catch {
    return -1;
  }
}

if (
  typeof process !== 'undefined' &&
  process.platform === 'win32' &&
  process.arch === 'x64' &&
  getWindowsMajorVersion() >= 10
) {
  try {
    module.exports = require('./binding.node');
  } catch (error) {
    console.error(error);
    module.exports = require('./NodeRT_Windows_Security_Credentials_UI.d');
  }
} else {
  module.exports = require('./NodeRT_Windows_Security_Credentials_UI.d');
}

const externalReferencedNamespaces = ['Windows.Storage.Streams'];

if (externalReferencedNamespaces.length > 0) {
  let namespaceRegistry = global.__winRtNamespaces__;

  if (!namespaceRegistry) {
    namespaceRegistry = {};

    Object.defineProperty(global, '__winRtNamespaces__', {
      configurable: true,
      writable: false,
      enumerable: false,
      value: namespaceRegistry,
    });
  }

  function requireNamespace(namespace) {
    let moduleName = namespace.toLowerCase();

    if (npmScope) {
      moduleName = `@${npmScope}/${moduleName}`;
    }

    const m = require(moduleName);
    delete namespaceRegistry[namespace];
    namespaceRegistry[namespace] = m;
    return m;
  }

  for (const i in externalReferencedNamespaces) {
    const ns = externalReferencedNamespaces[i];

    if (!namespaceRegistry.hasOwnProperty(ns)) {
      Object.defineProperty(namespaceRegistry, ns, {
        configurable: true,
        enumerable: true,
        get: requireNamespace.bind(null, ns),
      });
    }
  }
}
