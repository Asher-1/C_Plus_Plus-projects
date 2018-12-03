// PartTreeView.cpp : implementation file
//

#include "stdafx.h"
#include "STLViewer.h"
#include "PartTreeView.h"

#include "STLViewerDoc.h"
#include "DlgEntityName.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ITEM_OF_NAME	0
#define ITEM_OF_COLOR	1
#define ITEM_OF_VISIBLE	2

/////////////////////////////////////////////////////////////////////////////
// CPartTreeView

IMPLEMENT_DYNCREATE(CPartTreeView, CTreeView)

CPartTreeView::CPartTreeView()
{
}

CPartTreeView::~CPartTreeView()
{
}


BEGIN_MESSAGE_MAP(CPartTreeView, CTreeView)
	//{{AFX_MSG_MAP(CPartTreeView)
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPartTreeView drawing

void CPartTreeView::OnDraw(CDC* pDC)
{
	CSTLViewerDoc* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CPartTreeView diagnostics

#ifdef _DEBUG
void CPartTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CPartTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPartTreeView message handlers
CSTLViewerDoc* CPartTreeView::GetDocument()
{
	// 若该视图还未于文档关联
	if(m_pDocument==NULL){

		// 获取应用程序的文档指针
		CFrameWnd* frm = (CFrameWnd*) ::AfxGetMainWnd();
		ASSERT(frm);
		CDocument* pDoc = frm->GetActiveDocument();
		ASSERT(pDoc);

		// 确认获取的文档是所需要的类型
		ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(CSTLViewerDoc)));

		// 在文档中加入此视图的指针
		pDoc->AddView(this);
	}

	// 若已经与文档关联，则返回文档指针
	return (CSTLViewerDoc*)m_pDocument;
}

void CPartTreeView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	// 获取所关联的文档对象的指针
	CSTLViewerDoc* pDoc = GetDocument();
	ASSERT(pDoc);
	CTreeCtrl& treeCtrl = GetTreeCtrl();

	// 清空树型控件的所有条目
	treeCtrl.DeleteAllItems();

	HTREEITEM hti,hti0,hti1;

	CString str;
	// 插入根条目
	str.Format(_T("PART:%s"),pDoc->m_Part.GetName());
	hti = treeCtrl.InsertItem(str,0,0,NULL,TVI_LAST);
	
	CEntity* ent;

	// 根据文档中m_Part的内容创建树型控件的条目
	for(int i=0; i<pDoc->m_Part.GetEntitySize();i++){
		ent = pDoc->m_Part.GetEntity(i);
		ASSERT(ent);

		// 在根条目下插入STL模型的条目
		str.Format(_T("Model:%s"),ent->GetName());
		hti0 = treeCtrl.InsertItem(str,1,1,hti,TVI_LAST);

		// STL模型对象的指针值设为这个条目的数据
		// 通过这个数据使该条目和模型对象关联
		treeCtrl.SetItemData(hti0,(DWORD)ent);
		
		//attrib of name

		str.Format(_T("Name:%s"),ent->GetName());
		hti1 = treeCtrl.InsertItem(str,2,2,hti0,TVI_LAST);

		// 为名称属性条目设置数据ITEM_OF_NAME
		treeCtrl.SetItemData(hti1,ITEM_OF_NAME);

		//attrib of color 
		// 在STL模型条目下插入颜色属性条目
		COLORREF clr = ent->GetColor();
		BYTE r = GetRValue(clr);
		BYTE g = GetGValue(clr);
		BYTE b = GetBValue(clr);
		str.Format(_T("Color: R%d G%d B%d"),r,g,b);
		hti1 = treeCtrl.InsertItem(str,2,2,hti0,TVI_LAST);

		// 为颜色属性条目设置数据ITEM_OF_COLOR
		treeCtrl.SetItemData(hti1,ITEM_OF_COLOR);
		
		//attrib of visible
		// 在STL模型条目下插入显示属性条目
		if(ent->IsVisible())
			str = "Visible";
		else
			str = "Unvisible";
		hti1 = treeCtrl.InsertItem(str,2,2,hti0,TVI_LAST);

		// 为显示属性条目设置数据ITEM_OF_VISIBLE
		treeCtrl.SetItemData(hti1,ITEM_OF_VISIBLE);
	}

	// 将根目录下的条目展开
	GetTreeCtrl().Expand(hti,TVE_EXPAND);
}

void CPartTreeView::OnInitialUpdate() 
{
	CTreeView::OnInitialUpdate();
}

int CPartTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_ImageList.Create(IDB_PART_TREE, 16, 1, RGB(0,255,0));
	GetTreeCtrl().SetImageList(&m_ImageList, TVSIL_NORMAL);

	GetTreeCtrl().CancelToolTips(TRUE);
	CancelToolTips(TRUE);
	return 0;
}

void CPartTreeView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	UINT uFlags = 0;
	CTreeCtrl& treeCtrl = GetTreeCtrl();

	// 得到鼠标当前位置处的条目句柄
	HTREEITEM hItem = treeCtrl.HitTest(point,&uFlags);
	
	// 确认该条目是属性条目
	if(	hItem &&							// 必须有条目返回
		hItem != treeCtrl.GetRootItem() &&	// 返回的条目不为根条目
		!treeCtrl.ItemHasChildren(hItem)){	// 返回的条目没有子条目
		
		// 获取该条目的数据，即属性类别
		DWORD itemData = treeCtrl.GetItemData(hItem);
		
		// 获取该条目的父条目的句柄
		HTREEITEM hParentItem = treeCtrl.GetParentItem(hItem);
		ASSERT(hParentItem);

		// 获取父条目的数据，即指向CSTLModel子模型的指针
		DWORD parentData = treeCtrl.GetItemData(hParentItem);
		ASSERT(parentData);

		// 将父条目的数据转化为指向对象的指针，即将要被修改属性的对象的指针
		CEntity* ent = (CEntity*) parentData;

		// 根据所选条目的数据来判断修改什么属性
		switch(itemData){
		case ITEM_OF_NAME:
			ChangeEntityName(ent); // 修改名称属性
			break;
		case ITEM_OF_COLOR:
			ChangeEntityColor(ent); // 修改颜色属性
			break;
		case ITEM_OF_VISIBLE:
			ChangeEntityVisible(ent); // 修改显示属性
			break;
		}
	}
	CTreeView::OnLButtonDblClk(nFlags, point);
}

void CPartTreeView::ChangeEntityName(CEntity* ent)
{
	CSTLViewerDoc* pDoc = GetDocument();
	ASSERT(pDoc);
	CDlgEntityName dlg;
	dlg.m_EntityName = ent->GetName();
	if(dlg.DoModal() == IDOK){				// 弹出修改名称对话框
		ent->SetName(dlg.m_EntityName);		// 更新名称
		pDoc->UpdateAllViews(NULL);			// 更新视图显示
	}
}

// 修改颜色属性
void CPartTreeView::ChangeEntityColor(CEntity* ent)
{
	CSTLViewerDoc* pDoc = GetDocument();
	ASSERT(pDoc);
	CColorDialog dlg; // 定义颜色编辑对话框
	if(dlg.DoModal() == IDOK){ // 弹出修改颜色对话框
		COLORREF color = dlg.GetColor();
		ent->SetColor(color); // 更新名称
		pDoc->UpdateAllViews(NULL); // 更新视图
	}
}

void CPartTreeView::ChangeEntityVisible(CEntity* ent)
{
	CSTLViewerDoc* pDoc = GetDocument();
	ASSERT(pDoc);
	ent->SetVisible(!ent->IsVisible()); // 修改显示属性
	pDoc->UpdateAllViews(NULL); // 更新视图
}
