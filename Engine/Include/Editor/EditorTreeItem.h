#pragma once
#include "EditorWidget.h"

template <typename T>
class CEditorTreeItem
{
private:
	template <typename T>
	friend class CEditorTree;
	CEditorTreeItem<T>* m_Parent;
	std::vector<CEditorTreeItem<T>*>	m_vecChild;
	std::vector<CEditorWidget*>	m_vecWidget;
	ImGuiTreeNodeFlags	m_Flag;
	std::string		m_Item;
	std::string		m_ItemUTF8;
	T				m_CustomData;
	std::function<void(CEditorTreeItem<T>*, const std::string&)>	m_SelectCallback;
	CEditorTreeItem()
		: m_Parent(nullptr)
		, m_Flag(ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow)
		, m_CustomData{}
	{
	}
	~CEditorTreeItem()
	{
		size_t	size = m_vecChild.size();
		for (size_t i = 0; i < size; ++i)
		{
			SAFE_DELETE(m_vecChild[i]);
		}
		size = m_vecWidget.size();
		for (size_t i = 0; i < size; ++i)
		{
			SAFE_DELETE(m_vecWidget[i]);
		}
	}
public:
	T GetCustomData()	const
	{
		return m_CustomData;
	}
	void AddFlag(ImGuiTreeNodeFlags_ flag)
	{
		m_Flag |= flag;
	}
	void AddItem(const std::string& item, T customData)
	{
		CEditorTreeItem<T>* child = new CEditorTreeItem<T>;
		child->m_Parent = this;
		child->m_Item = item;
		child->m_SelectCallback = m_SelectCallback;
		child->m_CustomData = customData;
		TCHAR	wText[256] = {};
		int length = (int)MultiByteToWideChar(CP_ACP, 0, item.c_str(), -1, nullptr, 0);
		MultiByteToWideChar(CP_ACP, 0, item.c_str(), -1, wText, length);
		char	textUTF8[256] = {};
		length = WideCharToMultiByte(CP_UTF8, 0, wText, -1, nullptr, 0, nullptr, nullptr);
		WideCharToMultiByte(CP_UTF8, 0, wText, -1, textUTF8, length, nullptr, nullptr);
		child->m_ItemUTF8 = textUTF8;
		m_vecChild.push_back(child);
	}
	void AddItem(CEditorTreeItem<T>* item)
	{
		item->m_Parent = this;
		item->m_SelectCallback = m_SelectCallback;
		m_vecChild.push_back(item);
	}
	CEditorTreeItem<T>* FindItem(const std::string& item)
	{
		if (m_Item == item)
		{
			return this;
		}
		size_t	size = m_vecChild.size();
		for (size_t i = 0; i < size; ++i)
		{
			CEditorTreeItem<T>* find = m_vecChild[i]->FindItem(item);
			if (find)
			{
				return find;
			}
		}
		return nullptr;
	}
	void Clear()
	{
		size_t	size = m_vecChild.size();
		for (size_t i = 0; i < size; ++i)
		{
			m_vecChild[i]->Clear();
			SAFE_DELETE(m_vecChild[i]);
		}
		m_vecChild.clear();
	}
	void Render()
	{
		ImGuiTreeNodeFlags	flag = m_Flag;
		if (m_vecChild.empty())
		{
			flag |= ImGuiTreeNodeFlags_Leaf;
		}
		bool	itemOpen = ImGui::TreeNodeEx(m_ItemUTF8.c_str(), flag);
		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
		{
			if (m_SelectCallback)
			{
				m_SelectCallback(this, m_Item);
			}
		}
		size_t	widgetCount = m_vecWidget.size();
		for (size_t i = 0; i < widgetCount; ++i)
		{
			m_vecWidget[i]->Render();
		}
		/*if (ImGui::BeginDragDropSource())
		{
			ImGui::EndDragDropSource();
		}*/
		if (itemOpen)
		{
			size_t	size = m_vecChild.size();
			for (size_t i = 0; i < size; ++i)
			{
				m_vecChild[i]->Render();
			}
			ImGui::TreePop();
		}
	}
	template <typename CallbackType>
	void SetSelectCallback(CallbackType* obj, void (CallbackType::* func)(CEditorTreeItem<T>*, const std::string&))
	{
		m_SelectCallback = std::bind(func, obj, std::placeholders::_1, std::placeholders::_2);
	}
	template <typename WidgetType>
	WidgetType* CreateWidget(const std::string& name, float width = 100.f, float height = 100.f)
	{
		WidgetType* widget = new WidgetType;
		widget->SetName(name);
		widget->SetSize(width, height);
		if (!widget->Init())
		{
			SAFE_DELETE(widget);
			return nullptr;
		}
		m_vecWidget.push_back(widget);
		return widget;
	}
	template <typename WidgetType>
	WidgetType* FindWidget(const std::string& name)
	{
		size_t	size = m_vecWidget.size();
		for (size_t i = 0; i < size; ++i)
		{
			if (m_vecWidget[i]->GetName() == name)
			{
				return (WidgetType*)m_vecWidget[i];
			}
		}
		return nullptr;
	}
	template <typename WidgetType>
	WidgetType* FindWidgetHirearchy(const std::string& name)
	{
		WidgetType* widget = FindWidget<WidgetType>(name);
		if (widget)
		{
			return widget;
		}
		size_t	size = m_vecChild.size();
		for (size_t i = 0; i < size; ++i)
		{
			widget = m_vecChild[i]->FindWidgetHirearchy<WidgetType>(name);
			if (widget)
			{
				return widget;
			}
		}
		return nullptr;
	}
};

