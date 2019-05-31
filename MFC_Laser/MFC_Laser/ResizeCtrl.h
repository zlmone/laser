#include <Afxtempl.h>

class CWindowSizeMange
{
protected:
	typedef struct _tagSizeData_t
	{
		HWND hWndCtrl; //窗口句柄
		double fLeft, fTop, fRight, fBottom; //位置相对比例

		_tagSizeData_t()
		{
			memset(this, 0, sizeof(*this));
		}

		//初始化 (记录窗口原始坐标位置)
		void Init(HWND hParent, RECT rcParent, HWND hCtrl)
		{
			ASSERT(hParent && hCtrl);

			hWndCtrl = hCtrl;

			//获取控件的坐标
			RECT rcCtrl;
			GetWindowRect(hCtrl, &rcCtrl);
			POINT ptLT = { rcCtrl.left, rcCtrl.top };
			POINT ptRB = { rcCtrl.right, rcCtrl.bottom };
			ScreenToClient(hParent, &ptLT);
			ScreenToClient(hParent, &ptRB);

			//计算相对位置
			double cx = rcParent.right - rcParent.top;
			double cy = rcParent.bottom - rcParent.top;
			fLeft = ptLT.x / cx;
			fTop = ptLT.y / cy;
			fRight = ptRB.x / cx;
			fBottom = ptRB.y / cy;
		}

		//获取窗口坐标
		RECT GetRect(RECT rcParent)
		{
			RECT rcCtrl = { 0, 0, 0, 0 };
			if (hWndCtrl)
			{
				int cx = rcParent.right - rcParent.left;
				int cy = rcParent.bottom - rcParent.top;
				rcCtrl.left = (LONG)(cx * fLeft);
				rcCtrl.top = (LONG)(cy * fTop);
				rcCtrl.right = (LONG)(cx * fRight);
				rcCtrl.bottom = (LONG)(cy * fBottom);
			}

			return rcCtrl;
		}

	}SIZEDATA, *LPSIZEDATA;

protected:
	HWND m_hParentWnd;
	CArray <SIZEDATA, const SIZEDATA &>m_SizeArray;
	CRect m_rcOrgRect;

public:
	//初始化
	INT_PTR Init(HWND hParent)
	{
		ASSERT(hParent && IsWindow(hParent));

		m_hParentWnd = hParent;
		GetClientRect(hParent, &m_rcOrgRect);

		m_SizeArray.RemoveAll();
		HWND hWndT = NULL;
		while (1)
		{
			hWndT = FindWindowEx(hParent, hWndT, NULL, NULL);
			if (hWndT)
			{
				SIZEDATA m;
				m.Init(hParent, m_rcOrgRect, hWndT);
				m_SizeArray.Add(m);
			}
			else
			{
				break;
			}
		}

		return m_SizeArray.GetSize();
	}

	//缩放窗口控件
	void ResizeWindow()
	{
		CRect rcClient;
		GetClientRect(m_hParentWnd, &rcClient);
		if (m_hParentWnd && m_SizeArray.GetSize() > 0
			&& rcClient != m_rcOrgRect)
		{
			for (INT_PTR i = 0; i<m_SizeArray.GetSize(); i++)
			{
				SIZEDATA *pm = &m_SizeArray[i];

				CRect rcCtrl = pm->GetRect(rcClient);
				MoveWindow(pm->hWndCtrl,
					rcCtrl.left, rcCtrl.top, rcCtrl.Width(), rcCtrl.Height(), TRUE);
			}

			m_rcOrgRect = rcClient;
		}
	}
};