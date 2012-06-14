/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.40
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.sysfera.vishnu.api.vishnu.internal;

public class CreateDirOptions extends EObject {
  private long swigCPtr;

  protected CreateDirOptions(long cPtr, boolean cMemoryOwn) {
    super(VISHNUJNI.SWIGCreateDirOptionsUpcast(cPtr), cMemoryOwn);
    swigCPtr = cPtr;
  }

  protected static long getCPtr(CreateDirOptions obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        VISHNUJNI.delete_CreateDirOptions(swigCPtr);
      }
      swigCPtr = 0;
    }
    super.delete();
  }

  public CreateDirOptions() {
    this(VISHNUJNI.new_CreateDirOptions(), true);
  }

  public void _initialize() {
    VISHNUJNI.CreateDirOptions__initialize(swigCPtr, this);
  }

  public boolean isIsRecursive() {
    return VISHNUJNI.CreateDirOptions_isIsRecursive(swigCPtr, this);
  }

  public void setIsRecursive(boolean _isRecursive) {
    VISHNUJNI.CreateDirOptions_setIsRecursive(swigCPtr, this, _isRecursive);
  }

  public SWIGTYPE_p_ecorecpp__mapping__any eGet(int _featureID, boolean _resolve) {
    return new SWIGTYPE_p_ecorecpp__mapping__any(VISHNUJNI.CreateDirOptions_eGet(swigCPtr, this, _featureID, _resolve), true);
  }

  public void eSet(int _featureID, SWIGTYPE_p_ecorecpp__mapping__any _newValue) {
    VISHNUJNI.CreateDirOptions_eSet(swigCPtr, this, _featureID, SWIGTYPE_p_ecorecpp__mapping__any.getCPtr(_newValue));
  }

  public boolean eIsSet(int _featureID) {
    return VISHNUJNI.CreateDirOptions_eIsSet(swigCPtr, this, _featureID);
  }

  public void eUnset(int _featureID) {
    VISHNUJNI.CreateDirOptions_eUnset(swigCPtr, this, _featureID);
  }

  public SWIGTYPE_p_ecore__EClass _eClass() {
    long cPtr = VISHNUJNI.CreateDirOptions__eClass(swigCPtr, this);
    return (cPtr == 0) ? null : new SWIGTYPE_p_ecore__EClass(cPtr, false);
  }

}