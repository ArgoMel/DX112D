#pragma once
#include "EditorWidget.h"
#include "EditorTreeItem.h"

template <typename T>
class CEditorTree : public CEditorWidget
{
private:
	friend class CEditorWindow;
	friend class CEditorGroup;
	template <typename T>
	friend class CEditorTreeItem;
	CEditorTreeItem<T>* m_Root;
	std::function<void(CEditorTreeItem<T>*, const std::string&)>	m_SelectCallback;
	void NodeSelect(CEditorTreeItem<T>* node, const std::string& item)
	{
		if (m_SelectCallback)
		{
			m_SelectCallback(node, item);
		}
	}
protected:
	CEditorTree() 
		: m_Root(nullptr)
	{
	}
	virtual ~CEditorTree()
	{
		SAFE_DELETE(m_Root);
	}
public:
	bool AddItem(T customData, const std::string& item, const std::string& parentItem = "")
	{
		if (!m_Root)
		{
			m_Root = new CEditorTreeItem<T>;
			m_Root->m_Item = item;
			m_Root->m_CustomData = customData;
			TCHAR	wText[256] = {};
			int length = (int)MultiByteToWideChar(CP_ACP, 0, item.c_str(), -1, nullptr, 0);
			MultiByteToWideChar(CP_ACP, 0, item.c_str(), -1, wText, length);
			char	textUTF8[256] = {};
			length = WideCharToMultiByte(CP_UTF8, 0, wText, -1, nullptr, 0, nullptr, nullptr);
			WideCharToMultiByte(CP_UTF8, 0, wText, -1, textUTF8, length, nullptr, nullptr);
			m_Root->m_ItemUTF8 = textUTF8;
			m_Root->SetSelectCallback<CEditorTree<T>>(this, &CEditorTree<T>::NodeSelect);
			return true;
		}
		else
		{
			if (parentItem == "()")
			{
				CEditorTreeItem<T>* newItem = new CEditorTreeItem<T>;
				newItem->m_Item = item;
				newItem->m_CustomData = customData;
				TCHAR	wText[256] = {};
				int length = (int)MultiByteToWideChar(CP_ACP, 0, item.c_str(), -1, nullptr, 0);
				MultiByteToWideChar(CP_ACP, 0, item.c_str(), -1, wText, length);
				char	textUTF8[256] = {};
				length = WideCharToMultiByte(CP_UTF8, 0, wText, -1, nullptr, 0, nullptr, nullptr);
				WideCharToMultiByte(CP_UTF8, 0, wText, -1, textUTF8, length, nullptr, nullptr);
				newItem->m_ItemUTF8 = textUTF8;
				newItem->SetSelectCallback<CEditorTree<T>>(this, &CEditorTree<T>::NodeSelect);
				newItem->AddItem(m_Root);
				m_Root = newItem;
				return true;
			}
			else
			{
				CEditorTreeItem<T>* parent = m_Root->FindItem(parentItem);
				if (parent)
				{
					parent->AddItem(item, customData);
					return true;
				}
			}
		}
		return false;
	}
	void DeleteItem(const std::string& item)
	{
		if (!m_Root)
		{
			return;
		}
		CEditorTreeItem<T>* find = m_Root->FindItem(item);
		if (!find)
		{
			return;
		}
		CEditorTreeItem<T>* parent = find->m_Parent;
		if (!parent)
		{
			CEditorTreeItem<T>* root = m_Root->m_vecChild[0];
			size_t	size = m_Root->m_vecChild.size();
			for (size_t i = 1; i < size; ++i)
			{
				root->AddItem(m_Root->m_vecChild[i]);
			}
			m_Root->m_vecChild.clear();
			root->m_Parent = nullptr;
			SAFE_DELETE(m_Root);
			m_Root = root;
		}
		else
		{
			if (parent->m_vecChild.empty())
			{
				SAFE_DELETE(m_Root);
			}
			else
			{
				size_t	size = find->m_vecChild.size();
				parent->m_vecChild.clear();
				for (size_t i = 1; i < size; ++i)
				{
					parent->m_vecChild.push_back(find->m_vecChild[i]);
				}
				find->m_vecChild.clear();
				SAFE_DELETE(find);
			}
		}
	}
	void Clear()
	{
		if (m_Root)
		{
			m_Root->Clear();
			SAFE_DELETE(m_Root);
		}
	}
	virtual bool Init()
	{
		return true;
	}
	virtual void Render()
	{
		if (!m_Root)
		{
			return;
		}
		m_Root->Render();
	}
	template <typename CallbackType>
	void SetSelectCallback(CallbackType* obj, void(CallbackType::* func)(CEditorTreeItem<T>*, const std::string&))
	{
		m_SelectCallback = std::bind(func, obj, std::placeholders::_1, std::placeholders::_2);
	}
	template <typename WidgetType>
	WidgetType* CreateWidget(const std::string& itemName, const std::string& name, float width = 100.f, float height = 100.f)
	{
		if (!m_Root)
		{
			return nullptr;
		}
		CEditorTreeItem<T>* item = m_Root->FindItem(itemName);
		return item->CreateWidget<WidgetType>(name, width, height);
	}
	template <typename WidgetType>
	WidgetType* FindWidget(const std::string& itemName, const std::string& name)
	{
		if (!m_Root)
		{
			return nullptr;
		}
		CEditorTreeItem<T>* item = m_Root->FindItem(itemName);
		if (!item)
		{
			return nullptr;
		}
		return item->FindWidget<WidgetType>(name);
	}
	template <typename WidgetType>
	WidgetType* FindWidget(const std::string& name)
	{
		if (!m_Root)
		{
			return nullptr;
		}
		return m_Root->FindWidgetHirearchy<WidgetType>(name);
	}
};

