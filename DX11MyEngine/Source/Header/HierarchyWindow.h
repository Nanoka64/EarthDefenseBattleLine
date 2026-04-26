#pragma once
#include "EditWindowBase.h"
class HierarchyWindow :   public EditWindowBase
{
private:
    std::unordered_map<std::string, std::unique_ptr<class EditorBase>> m_EditorMap;

public:
    HierarchyWindow();
    ~HierarchyWindow() override;
    bool Init(RendererEngine &renderer) override;
    void Update(RendererEngine &renderer)override;

private:
    void SelectObjectsDraw(std::vector<std::shared_ptr<GameObject>>& _objects)const;

};

