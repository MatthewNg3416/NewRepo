#pragma once
#include "MFileManager.h" //여기 상대 경로로 사용해야함

class IDataType :
	public MFileManager
{
public:
	IDataType() { m_bModelData = false; m_bChanged = false; m_bHardCode = false; m_bRedingData = false; }
	~IDataType() {}

	//Set Model Data
public:
	virtual void SetModelData() { m_bModelData = true; }
	virtual void ResetModelData() { m_bModelData = false; }
	virtual bool IsModelData() { return m_bModelData; }
private:
	bool m_bModelData;

protected:
	bool m_bRedingData;
public:
	virtual CString GetCStringData() = 0;
	virtual void SetCStringData(CString str) = 0;
	int ReadData()
	{
		if (IsHardCode())
		{
			UpdateHardCodingData();
			m_bRedingData = true;
			return true;
		}

		bool bReturn;
		CString	strSection;
		CString	strItem = Name;
		CString strTemp;

		bReturn = GetValue(strSection, strItem, &strTemp);
		if (bReturn == false)
		{
			do {
				//if (FALSE == DisplayDataReadError(strSection, strItem, m_strFileName)) 
				//{ 
				//	bReturn = FALSE;
				//	return FALSE; 
				//}
				bReturn = TRUE;
			} while (0);
		}

		SetCStringData(strTemp);

		m_bRedingData = true;
		return true;
	}

	int WriteData()
	{
		if (m_bChanged == false)
			return true;

		CString strSection;
		CString strItem = Name;
		CString strTemp;
		bool bReturn = TRUE;

		strTemp = GetCStringData();

		bReturn = SetValue(strSection, strItem, strTemp);
		//if (!bReturn)
		//	DISPLAY_WRITE_ERROR(strSection, strItem, bReturn);

		m_bChanged = false;
		return true;
	}
	//hardcodeing
public:
	virtual bool IsHardCode() { return m_bHardCode; }
protected:
	bool m_bHardCode;

	//changed
public:
	virtual bool IsChanged() { return m_bChanged; }
protected:
	bool m_bChanged;

	//name
public:
	virtual void SetName(CString str) { Name = str; }
	virtual CString GetName() { return Name; }
protected:
	CString Name;
public:
	virtual IDataType* Initlize(CString strName, bool bModelData = false) 
	{ 
		SetName(strName); 
		if (bModelData)
			SetModelData();
		return this; 
	}

	virtual IDataType* Initlize(CString strName, int i, bool bModelData = false)
	{
		CString strTemp;
		strTemp.Format(_T("%s[%d]"), strName, i);
		SetName(strTemp);
		if (bModelData)
			SetModelData();
		return this;
	}
public:
	virtual void CancelChange() = 0;
	virtual void InputTool() = 0;
	virtual void UpdateHardCodingData() = 0;
};


class MDataDouble :
	public IDataType
{
private:
	double m_Data;
	double m_OldData;
	double m_HardData;
public:
	MDataDouble(){ m_Data = 0; }
	~MDataDouble() {}
	void InputTool();

	void UpdateHardCodingData() { SetData(m_HardData); }
	void SetHardCodeData(double Data) 
	{ 
		m_bHardCode = true; 
		m_HardData = Data;
		UpdateHardCodingData();
	}
	void ResetHardCodeData() { m_bHardCode = false; m_HardData = 0; }
	
	void SetData(double Data)
	{
		if (m_Data == Data)
			return;
		
		if (m_bChanged == false)
			m_OldData = m_Data;

		if (IsHardCode())
			m_Data = m_HardData;
		else
			m_Data = Data;

		if (m_bRedingData == true)
			m_bChanged = true;
	}

	double GetData()
	{
		if (m_bRedingData == false)
			return 0;
		if (IsHardCode())
			if (m_Data != m_HardData)
				UpdateHardCodingData();

		return m_Data;
	}

	CString GetCStringData()
	{
		CString strValue;
		strValue.Format(_T("%lf"), m_Data);
		return strValue;
	}
	void SetCStringData(CString str)
	{
		m_Data = wcstod(str, NULL);
	}
	
	void CancelChange()
	{
		if (m_bChanged == true)
		{
			m_Data = m_OldData;
			m_bChanged = false;
		}
	}
};


class MDataInt:
	public IDataType
{
private:
	int m_Data;
	int m_OldData;
	int m_HardData;
public:
	MDataInt() { m_Data = 0; }
	~MDataInt() {}
	void InputTool();

	void UpdateHardCodingData() { SetData(m_HardData); }
	void SetHardCodeData(int Data)
	{ 
		m_bHardCode = true; 
		m_HardData = Data; 
		UpdateHardCodingData();
	}
	void ResetHardCodeData() { m_bHardCode = false; m_HardData = 0; }

	void SetData(int Data)
	{
		if (m_Data == Data)
			return;

		if (m_bChanged == false)
			m_OldData = m_Data;

		if (IsHardCode())
			m_Data = m_HardData;
		else
			m_Data = Data;

		if (m_bRedingData == true)
			m_bChanged = true;
	}

	int GetData()
	{
		if (m_bRedingData == false)
			return 0;

		if (IsHardCode())
			if (m_Data != m_HardData)
				UpdateHardCodingData();

		return m_Data;
	}

	CString GetCStringData()
	{
		CString strValue;
		strValue.Format(_T("%d"), m_Data);
		return strValue;
	}
	void SetCStringData(CString str)
	{
		m_Data = _wtoi(str);
	}

	void CancelChange()
	{
		if (m_bChanged == true)
		{
			m_Data = m_OldData;
			m_bChanged = false;
		}
	}
};


class MDataBool :
	public IDataType
{
private:
	bool m_Data;
	bool m_OldData;
	bool m_HardData;
public:
	MDataBool() { m_Data = 0; }
	~MDataBool() {}
	void InputTool();

	void UpdateHardCodingData() { SetData(m_HardData); }
	void SetHardCodeData(bool Data)
	{
		m_bHardCode = true;
		m_HardData = Data;
		UpdateHardCodingData();
	}
	void ResetHardCodeData() { m_bHardCode = false; m_HardData = 0; }

	void SetData(bool Data)
	{
		if (m_Data == Data)
			return;

		if (m_bChanged == false)
			m_OldData = m_Data;

		if (IsHardCode())
			m_Data = m_HardData;
		else
			m_Data = Data;

		if (m_bRedingData == true)
			m_bChanged = true;
	}

	bool GetData()
	{
		if (m_bRedingData == false)
			return 0;

		if (IsHardCode())
			if (m_Data != m_HardData)
				UpdateHardCodingData();

		return m_Data;
	}

	CString GetCStringData() 
	{
		if (m_Data == TRUE)
			return _T("TRUE");
		else
			return _T("FALSE");
	}

	void SetCStringData(CString strTemp)
	{
		if (strTemp.Compare(_T("FALSE")) == 0)
			m_Data = FALSE;
		else
			m_Data = TRUE;
	}

	void CancelChange()
	{
		if (m_bChanged == true)
		{
			m_Data = m_OldData;
			m_bChanged = false;
		}
	}
};


class MDataCString :
	public IDataType
{
private:
	CString m_Data;
	CString m_OldData;
	CString m_HardData;
public:
	MDataCString() {}
	~MDataCString() {}
	void InputTool();

	void UpdateHardCodingData() { SetData(m_HardData); }
	void SetHardCodeData(CString Data)
	{
		m_bHardCode = true;
		m_HardData = Data;
		UpdateHardCodingData();
	}
	void ResetHardCodeData() { m_bHardCode = false;}

	void SetData(CString Data)
	{
		if (m_Data == Data)
			return;

		if (m_bChanged == false)
			m_OldData = m_Data;

		if (IsHardCode())
			m_Data = m_HardData;
		else
			m_Data = Data;

		if(m_bRedingData == true)
			m_bChanged = true;
	}

	CString GetData()
	{
		if (m_bRedingData == false)
			return _T("");

		if (IsHardCode())
			if (m_Data != m_HardData)
				UpdateHardCodingData();

		return m_Data;
	}

	void CancelChange()
	{
		if (m_bChanged == true)
		{
			m_Data = m_OldData;
			m_bChanged = false;
		}
	}

public:
	CString GetCStringData() { return GetData(); }
	void SetCStringData(CString str) { SetData(str); }
};