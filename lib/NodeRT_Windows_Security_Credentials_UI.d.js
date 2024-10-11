_AuthenticationProtocol = function () {
  this.basic = 0;
  this.digest = 1;
  this.ntlm = 2;
  this.kerberos = 3;
  this.negotiate = 4;
  this.credSsp = 5;
  this.custom = 6;
}
exports.AuthenticationProtocol = new _AuthenticationProtocol();

_CredentialSaveOption = function () {
  this.unselected = 0;
  this.selected = 1;
  this.hidden = 2;
}
exports.CredentialSaveOption = new _CredentialSaveOption();

_UserConsentVerificationResult = function () {
  this.verified = 0;
  this.deviceNotPresent = 1;
  this.notConfiguredForUser = 2;
  this.disabledByPolicy = 3;
  this.deviceBusy = 4;
  this.retriesExhausted = 5;
  this.canceled = 6;
}
exports.UserConsentVerificationResult = new _UserConsentVerificationResult();

_UserConsentVerifierAvailability = function () {
  this.available = 0;
  this.deviceNotPresent = 1;
  this.notConfiguredForUser = 2;
  this.disabledByPolicy = 3;
  this.deviceBusy = 4;
}
exports.UserConsentVerifierAvailability = new _UserConsentVerifierAvailability();

CredentialPicker = (function () {
  var cls = function CredentialPicker() {
  };
  

  cls.pickAsync = function pickAsync(options, callback) {
    /// <signature>
    /// <summary>Function summary.</summary>
    /// <param name="options" type="CredentialPickerOptions">A param.</param>
    /// </signature>
  }

cls.pickAsync = function pickAsync(targetName, message, callback) {
    /// <signature>
    /// <summary>Function summary.</summary>
    /// <param name="targetName" type="String">A param.</param>
    /// <param name="message" type="String">A param.</param>
    /// </signature>
  }

cls.pickAsync = function pickAsync(targetName, message, caption, callback) {
    /// <signature>
    /// <summary>Function summary.</summary>
    /// <param name="targetName" type="String">A param.</param>
    /// <param name="message" type="String">A param.</param>
    /// <param name="caption" type="String">A param.</param>
    /// </signature>
  }



  return cls;
}) ();
exports.CredentialPicker = CredentialPicker;

CredentialPickerOptions = (function () {
  var cls = function CredentialPickerOptions() {
    this.targetName = new String();
    this.previousCredential = new Object();
    this.message = new String();
    this.errorCode = new Number();
    this.customAuthenticationProtocol = new String();
    this.credentialSaveOption = new CredentialSaveOption();
    this.caption = new String();
    this.callerSavesCredential = new Boolean();
    this.authenticationProtocol = new AuthenticationProtocol();
    this.alwaysDisplayDialog = new Boolean();
  };
  

  return cls;
}) ();
exports.CredentialPickerOptions = CredentialPickerOptions;

CredentialPickerResults = (function () {
  var cls = function CredentialPickerResults() {
    this.credential = new Object();
    this.credentialDomainName = new String();
    this.credentialPassword = new String();
    this.credentialSaveOption = new CredentialSaveOption();
    this.credentialSaved = new Boolean();
    this.credentialUserName = new String();
    this.errorCode = new Number();
  };
  

  return cls;
}) ();
exports.CredentialPickerResults = CredentialPickerResults;

UserConsentVerifier = (function () {
  var cls = function UserConsentVerifier() {
  };
  

  cls.checkAvailabilityAsync = function checkAvailabilityAsync(callback) {
    /// <signature>
    /// <summary>Function summary.</summary>
    /// </signature>
    callback(null, 1);
  }



  cls.requestVerificationAsync = function requestVerificationAsync(message, callback) {
    /// <signature>
    /// <summary>Function summary.</summary>
    /// <param name="message" type="String">A param.</param>
    /// </signature>
    callback(null, 1);
  }



  return cls;
}) ();
exports.UserConsentVerifier = UserConsentVerifier;

