#undef UNICODE
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <AclAPI.h>
#include <sddl.h>
#include <string>

using namespace std;

const int MAX_LINE = 50;
const int READ_REG_DWORD = 0;
const int WRITE_REG_DWORD = 1;
const int READ_REG_STRING = 2;
const int WRITE_REG_STRING = 3;
const int FIND_REG_KEY = 4;
const int SHOW_SUBKEYS = 5;
const int READ_KEY_FLAG = 6;
const int EXIT = 7;
const int NOTIFY_KEY_CHANGED = 8;

HANDLE hKeyChangedEvent;
HANDLE hKeyChangedThread;

int findKey(HKEY currentKey, LPCSTR keyName);
void showSukbeys(HKEY currentKey);
int readKeyFlags(HKEY currentKey);
LPCSTR readStringValue(HKEY key, LPCSTR subkey, LPCSTR valueName);
DWORD writeStringValue(HKEY key, LPCSTR subkey, LPCSTR valueName, LPCSTR value);
DWORD writeDWORDValue(HKEY key, LPCSTR subkey, LPCSTR valueName, DWORD value);
DWORD readDWORDValue(HKEY key, LPCSTR subkey, LPCSTR valueName);
void CreateThreadAndEvent();
DWORD WINAPI onKeyChanged(LPVOID lpParam);

void notifyKeyChanged(HKEY currentKey)
{
	CreateThreadAndEvent();
	RegNotifyChangeKeyValue(currentKey, TRUE, REG_NOTIFY_CHANGE_LAST_SET, hKeyChangedEvent, TRUE);
}

int findKey(HKEY currentKey, LPCSTR keyName)
{
	DWORD subkeysAmount;
	DWORD maxSubkeyLen, currentSubkeyLen;
	DWORD result;
	RegQueryInfoKey(currentKey, NULL, 0, NULL, &subkeysAmount, &maxSubkeyLen, NULL, NULL, NULL, NULL, NULL, NULL);
	maxSubkeyLen = 1024;
	char* bufferName = new char[maxSubkeyLen];
	for (int i = 0; i < subkeysAmount; i++)
	{
		currentSubkeyLen = maxSubkeyLen;
		result = RegEnumKeyEx(currentKey, i, bufferName, &currentSubkeyLen, NULL, NULL, NULL, NULL);
		if (result == ERROR_SUCCESS)
		{
			if (!strcmp(bufferName, keyName))
			{
				return 1;
			}
			HKEY innerKey;
			result = RegOpenKey(currentKey, bufferName, &innerKey);
			if (result == ERROR_SUCCESS)
			{
				result = findKey(innerKey, keyName);
				if (result)
				{
					RegCloseKey(innerKey);
					return result;
				}
			}
			RegCloseKey(innerKey);
		}
	}
	return 0;
}

void showSukbeys(HKEY currentKey)
{
	DWORD subkeysAmount;
	DWORD maxSubkeyLen, currentSubkeyLen;
	DWORD result;
	RegQueryInfoKey(currentKey, NULL, 0, NULL, &subkeysAmount, &maxSubkeyLen, NULL, NULL, NULL, NULL, NULL, NULL);
	maxSubkeyLen = 1024;
	char* bufferName = new char[maxSubkeyLen];
	if (subkeysAmount > 0)
		printf("Subkeys: \n");
	else
		printf("No subkeys\n");
	for (int i = 0; i < subkeysAmount; i++)
	{
		currentSubkeyLen = maxSubkeyLen;
		result = RegEnumKeyEx(currentKey, i, bufferName, &currentSubkeyLen, NULL, NULL, NULL, NULL);
		if (result != ERROR_SUCCESS)
		{
			printf("Can not get subkey\n");
		}
		else
		{
			printf("%s\n", bufferName);
		}
	}
}

const char* parseAceString(char* source)
{
	string strSource(source);
	string* strKeyAccess = new string("");
	int strLen = strSource.length();
	int i = 0;
	int semicolonCounter = 0;
	while (i < strLen)
	{
		if (source[i] == ')')
		{
			semicolonCounter = 0;
		}
		if (source[i] == ';')
		{
			semicolonCounter++;
		}
		if (semicolonCounter == 2)
		{
			int start = i + 1;
			do
			{
				i++;
			} 
			while (source[i] != ';');
			strKeyAccess->append(strSource.substr(start, i - start));
			semicolonCounter++;
		}
		if (semicolonCounter == 5)
		{
			int start = i + 1;
			while (source[i] != ')')
			{
				i++;
			}
			strKeyAccess->append(" <-" + strSource.substr(start, i - start) + "\n");
			semicolonCounter = 0;
		}
		i++;
	}
	return strKeyAccess->c_str();
}

int readKeyFlags(HKEY currentKey)
{
	int isSuccess = 1;
	DWORD securityDescriptorSize;
	DWORD subkeysNumber;
	RegQueryInfoKey(currentKey, NULL, 0, NULL, &subkeysNumber, NULL, NULL, NULL, NULL, NULL, &securityDescriptorSize, NULL);
	char* buffer = new char[securityDescriptorSize];
	DWORD result;
	result = RegGetKeySecurity(currentKey, DACL_SECURITY_INFORMATION, buffer, &securityDescriptorSize);
	if (result != ERROR_SUCCESS)
	{
		printf("Can not get security\n");
		isSuccess = 0;
	}
	else
	{
		SECURITY_DESCRIPTOR* security = reinterpret_cast<SECURITY_DESCRIPTOR*>(buffer);
		LPSTR strDacl;
		ConvertSecurityDescriptorToStringSecurityDescriptor(security, SDDL_REVISION_1, DACL_SECURITY_INFORMATION, &strDacl, NULL);
		printf("%s\n", parseAceString(strDacl));
	}
	return isSuccess;
}

LPCSTR readStringValue(HKEY key, LPCSTR subkey, LPCSTR valueName)
{
	DWORD result;
	HKEY openedKey;
	result = RegOpenKeyEx(key, subkey, 0, KEY_READ, &openedKey);
	if (result != ERROR_SUCCESS)
	{
		printf("Can not open key");
		return NULL;
	}
	char* buffer = new char[MAX_LINE];
	DWORD len = MAX_LINE;
	result = RegQueryValueEx(openedKey, valueName, NULL, NULL, (BYTE*)buffer, &len);
	if (result != ERROR_SUCCESS)
	{
		printf("Can not read value");
		return NULL;
	}
	RegCloseKey(openedKey);
	return buffer;;
}

DWORD writeStringValue(HKEY key, LPCSTR subkey, LPCSTR valueName, LPCSTR value)
{
	DWORD disposition;
	HKEY openedKey;
	DWORD result;
	result = RegCreateKeyEx(HKEY_CURRENT_USER, subkey, 0, REG_OPTION_NON_VOLATILE, NULL, KEY_ALL_ACCESS, NULL, &openedKey, &disposition);
	if (result != ERROR_SUCCESS)
	{
		printf("Can not create key");
		return -1;
	}
	result = RegSetValueEx(openedKey, valueName, 0, REG_SZ, (BYTE*)value, strlen(value));
	if (result != ERROR_SUCCESS)
	{
		printf("Can not write registry");
		return -1;
	}
	RegCloseKey(openedKey);
	return 0;
}

DWORD readDWORDValue(HKEY key, LPCSTR subkey, LPCSTR valueName)
{
	DWORD result;
	HKEY openedKey;
	result = RegOpenKeyEx(key, subkey, 0, KEY_READ, &openedKey);
	if (result != ERROR_SUCCESS)
	{
		printf("Can not open key");
		return -1;
	}
	DWORD buffer;
	DWORD len = sizeof(buffer);
	result = RegQueryValueEx(openedKey, valueName, NULL, NULL, (BYTE*)&buffer, &len);
	if (result != ERROR_SUCCESS)
	{
		printf("Can not read value");
		return -1;
	}
	RegCloseKey(openedKey);
	return buffer;
}

DWORD writeDWORDValue(HKEY key, LPCSTR subkey, LPCSTR valueName, DWORD value)
{
	DWORD disposition;
	HKEY openedKey;
	DWORD result;
	result = RegCreateKeyEx(HKEY_CURRENT_USER, subkey, 0, REG_OPTION_NON_VOLATILE, NULL, KEY_ALL_ACCESS, NULL, &openedKey, &disposition);
	if (result != ERROR_SUCCESS)
	{
		printf("Can not create key");
		return -1;
	}
	result = RegSetValueEx(openedKey, valueName, 0, REG_DWORD, (BYTE*)&value, sizeof(DWORD));
	if (result != ERROR_SUCCESS)
	{
		printf("Can not write registry");
		return -1;
	}
	RegCloseKey(openedKey);
	return 0;
}

void CreateThreadAndEvent()
{
	hKeyChangedEvent = CreateEvent(NULL, TRUE, FALSE, "KeyChanged");
	DWORD threadId;
	hKeyChangedThread = CreateThread(NULL, 0, onKeyChanged, NULL, 0, &threadId);
}

void CloseEvents()
{
	CloseHandle(hKeyChangedEvent);
}

DWORD WINAPI onKeyChanged(LPVOID lpParam)
{
	WaitForSingleObject(hKeyChangedEvent, INFINITE);
	printf("Key changed\n");
	return 0;
}

int main()
{
	string rules = "0 - read dword value\n";
	rules += "1 - write dword value\n";
	rules += "2 - read string valie\n";
	rules += "3 - write string value\n";
	rules += "4 - find key\n";
	rules += "5 - show subkeys\n";
	rules += "6 - read key flags\n";
	rules += "7 - exit\n";
	rules += "8 - notify key changed\n";
	printf("%s\n", rules.c_str());
	int isContinue = 1;
	char buffer[1024];
	char secondBuffer[1024];
	DWORD dwValue;
	DWORD dwResult;
	LPCSTR strResult;
	HKEY currentKey;
	DWORD disposition;
	LPCSTR subkey = "TestKey";
	while (isContinue)
	{
		int command;
		scanf("%d", &command);
		switch (command)
		{
		case READ_REG_DWORD:
			printf("Enter value name: ");
			scanf("%s", buffer);
			dwResult = readDWORDValue(HKEY_CURRENT_USER, subkey, buffer);
			if (dwResult != -1)
			{
				printf("Value: %d\n", dwResult);
			}
			break;
		case WRITE_REG_DWORD:
			printf("Enter value name: ");
			scanf("%s", buffer);
			printf("Enter value: ");
			scanf("%d", &dwValue);
			writeDWORDValue(HKEY_CURRENT_USER, subkey, buffer, dwValue);
			break;
		case READ_REG_STRING:
			printf("Enter value name: ");
			scanf("%s", buffer);
			strResult = readStringValue(HKEY_CURRENT_USER, subkey, buffer);
			if (strResult != NULL)
			{
				printf("Value: %s\n", strResult);
			}
			break;
		case WRITE_REG_STRING:
			printf("Enter value name: ");
			scanf("%s", buffer);
			printf("Enter value: ");
			scanf("%s", secondBuffer);
			writeStringValue(HKEY_CURRENT_USER, subkey, buffer, secondBuffer);
			break;
		case FIND_REG_KEY:
			printf("Enter key name to find: ");
			scanf("%s", buffer);
			if (findKey(HKEY_CURRENT_USER, buffer))
			{
				printf("Found!\n");
			}
			else
			{
				printf("Not found\n");
			}
			break;
		case SHOW_SUBKEYS:
		{
			HKEY currentKey;
			printf("Enter key name: ");
			scanf("%s", buffer);
			dwResult = RegOpenKey(HKEY_CURRENT_USER, buffer, &currentKey);
			if (dwResult != ERROR_SUCCESS)
			{
				printf("Can not open key\n");
			}
			else
			{
				showSukbeys(currentKey);
				RegCloseKey(currentKey);
			}
		}
		break;
		case READ_KEY_FLAG:
		{
			HKEY currentKey;
			printf("Enter the key name: ");
			scanf("%s", buffer);
			dwResult = RegOpenKey(HKEY_CURRENT_USER, buffer, &currentKey);
			if (dwResult != ERROR_SUCCESS)
			{
				printf("Can not open key\n");
			}
			else
			{
				readKeyFlags(currentKey);
				RegCloseKey(currentKey);
			}
		}
		break;
		case NOTIFY_KEY_CHANGED:
		{
			printf("Enter the key name:");
			scanf("%s", buffer);
			HKEY currentKey;
			dwResult = RegOpenKey(HKEY_CURRENT_USER, buffer, &currentKey);
			if (dwResult != ERROR_SUCCESS)
			{
				printf("Can not open key\n");
			}
			else
			{
				notifyKeyChanged(currentKey);
			}
		}
		break;
		case EXIT:
			isContinue = 0;
			break;
		default:
			break;
		}
	}
	return 0;
}