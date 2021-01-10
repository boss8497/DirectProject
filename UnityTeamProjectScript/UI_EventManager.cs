using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UI_EventManager : MonoBehaviour
{
    /**//**//**//**//**//**//**//**//**//**//**//**/
    private static UI_EventManager _instance = null;
    public static UI_EventManager Instance
    {
        get
        {
            if (_instance == null)
            {
                _instance = FindObjectOfType(typeof(UI_EventManager)) as UI_EventManager;

                if (_instance == null)
                {
                    Debug.LogError("There's no active ManagerClass object");
                }
            }
            return _instance;
        }
    }
   

    private Display_Surround _Surround = Display_Surround.NONE;
    public Display_Surround Surrounnd
    {
        get { return _Surround; }
        set { _Surround = value; }
    }

    public Vector2Int ScreenSize = new Vector2Int();

    public UIControl UIcontrol;
    public GameObject BaseMenu;

    public GameObject[] ALL_UI_Canvas;

    Stack<GameObject> PanelStack = new Stack<GameObject>();

    public HashSet<UIPosition> ChiledPos = new HashSet<UIPosition>();
    public HashSet<UIPosition> AllUI = new HashSet<UIPosition>();

    public float CarRotateSpeed = 1.5f;
    public float StatBarSpeed = 1.5f;

    public GameResult gameResult = GameResult.NONE;
    public Camera MainCamera;
    public GameObject LeftView;
    public GameObject RightView;
    public static bool IsCheckScreenResolution = false;

    private void Awake()
    {
        CheckScreenSize();
        CheckOldDonDestroy();

    }
    private void Start()
    {
        InitePanel();
    }


    private void Update()
    {
        if(ChiledPos.Count > 0)
        {
            PosSettingChiled();
        }

        if (UIcontrol)
        {
            UIcontrol.RInput.InputCheck();
            UIcontrol.InputControll();
        }
    }

    public void CheckOldDonDestroy()
    {
        MapCarSelectData[] obj = FindObjectsOfType<MapCarSelectData>();
        foreach (var m in obj)
        {
            if (m.IsOld)
                Object.Destroy(m.gameObject);
        }
    }
    




    //이벤트 관련
    public void PanelPush(GameObject _panel)
    {
        PanelStack.Push(_panel);
    }
    public bool PanelPop()
    {
        //index = 1 , baseMenu
        if (PanelStack.Count == 1)
            return false;


        PanelStack.Pop();
        return true;
    }
    public GameObject PanelPeek()
    {
        return PanelStack.Peek();
    }
    public void ChangeResult()
    {
        UIcontrol.panelinfo = PanelPeek().GetComponent<PanelInfo>();

        switch (UIcontrol.panelinfo.name)
        {
            case "ModeSelectPanel":
                break;

            default:
                UIcontrol.panelinfo.PointerIndex = 0;
                break;
        }


        switch (gameResult)
        {
            case GameResult.RETIRE:
                {
                    if (UIcontrol.panelinfo.name == "ResultCanvas")
                    {
                        UIcontrol.MainMenu = UIcontrol.panelinfo.SubMenu;
                        UIcontrol.Pointer.position = UIcontrol.MainMenu[UIcontrol.panelinfo.PointerIndex].position;
                    }
                    else
                    {
                        UIcontrol.MainMenu = UIcontrol.panelinfo.Menu;
                        UIcontrol.Pointer.position = UIcontrol.MainMenu[UIcontrol.panelinfo.PointerIndex].position;
                    }
                }
                break;

            case GameResult.FINISH:
            case GameResult.NONE:
                {
                    UIcontrol.MainMenu = UIcontrol.panelinfo.Menu;
                    UIcontrol.Pointer.position = UIcontrol.MainMenu[UIcontrol.panelinfo.PointerIndex].position;
                }
                break;
        }
    }
    public void UIControlClosePanelMenuChange()
    {
        UIcontrol.panelinfo = PanelPeek().GetComponent<PanelInfo>();

        switch (gameResult)
        {
            case GameResult.RETIRE:
                {
                    if (UIcontrol.panelinfo.name == "ResultCanvas")
                    {
                        UIcontrol.MainMenu = UIcontrol.panelinfo.SubMenu;
                        UIcontrol.Pointer.position = UIcontrol.MainMenu[UIcontrol.panelinfo.PointerIndex].position;
                    }
                    else
                    {
                        UIcontrol.MainMenu = UIcontrol.panelinfo.Menu;
                        UIcontrol.Pointer.position = UIcontrol.MainMenu[UIcontrol.panelinfo.PointerIndex].position;
                    }
                }
                break;

            case GameResult.FINISH:
            case GameResult.NONE:
                {
                    UIcontrol.MainMenu = UIcontrol.panelinfo.Menu;
                    UIcontrol.Pointer.position = UIcontrol.MainMenu[UIcontrol.panelinfo.PointerIndex].position;
                }
                break;
        }
    }
    public void InitePanel()
    {
        if (BaseMenu)
        {
            PanelPush(BaseMenu);
            BaseMenu.GetComponent<PanelInfo>().SetInitePosition();
            BaseMenu.GetComponent<PanelInfo>().InitePosition();
            switch (BaseMenu.name)
            {
                case "TitleCanvas":
                    StringData.Instance.ChangeTitleString(BaseMenu.GetComponent<PanelInfo>().PointerIndex);
                    break;
            }
            ChangeResult();

            switch (UIcontrol.panelinfo.mode)
            {
                case PanelMode.NONE:
                    break;
                case PanelMode.VIEW_SCROLL:
                    break;
                case PanelMode.MESSAGEBOX:
                    break;
                case PanelMode.Circle:
                    UIcontrol.MainMenu[0].GetComponent<Image>().sprite = UIcontrol.panelinfo.circleonoff.OnImage;
                    break;
                case PanelMode.CAR_SELECT:
                    break;
            }
        }
        
    }






    //UI위치 관련
    public void PosSettingChiled()
    {
        bool flag = true;
        while (flag)
        { 
            flag = false;
            if (ChiledPos.Count != 0)
            {
                foreach (UIPosition m in ChiledPos)
                {
                    if (m.Parent.GetComponent<UIPosition>().SettingPos)
                    {
                        m.SetParent();
                        m.SetPosSize();
                        ChiledPos.Remove(m);
                        flag = true;
                        break;
                    }
                }
            }
        }
        
    }
    private void CheckScreenSize()
    {
        Resolution[] resolutions = Screen.resolutions;

        ScreenSize.x = resolutions[resolutions.Length - 1].width;
        ScreenSize.y = resolutions[resolutions.Length - 1].height;

        if (!IsCheckScreenResolution)
        {
            Screen.SetResolution(ScreenSize.x, ScreenSize.y, true);
            IsCheckScreenResolution = true;
        }

        if (ScreenSize.x == 5760)
        {
            ScreenSize.x = 1920;
            _Surround = Display_Surround.DISPLAY_3;
        }

        

        if(Screen.width >= 5760)
        {
            if(MainCamera)
            {
                MainCamera.rect = new Rect(0.33333f, 0, 0.33333f, 1);
            }
            if (LeftView)
            {
                LeftView.SetActive(true);
            }

            if (RightView)
            {
                RightView.SetActive(true);
            }
        }
        else
        {

            if (MainCamera)
            {
                MainCamera.rect = new Rect(0, 0, 1, 1);
            }
            if (LeftView)
            {
                LeftView.SetActive(false);
            }

            if (RightView)
            {
                RightView.SetActive(false);
            }
        }


        /*에디터용*/
        ScreenSize.x = Screen.width;
        ScreenSize.y = Screen.height;

        if (ScreenSize.x == 5760)
        {
            ScreenSize.x = 1920;
            _Surround = Display_Surround.DISPLAY_3;
        }
    }
}

public enum UIEventList
{
    NONE,
    OpenPanel,
    NoneOpenPanel,
    ClosePanel,
    NoneClosePanel,
    NextScenes,
    OutClient,
    OpenMenu,
    CloseMenu,
    MessageBoxBackMenu,
    MapSelect,
    GameRestart,
    InputName,
    OpenAndSetActivity,
    CloseAndSetActivity,
    TitleScenes,
    GameStart
}
public enum PanelMode
{
    NONE,
    VIEW_SCROLL,
    MESSAGEBOX,
    Circle,
    CAR_SELECT,
    GAME_RESULT
}
public enum Anchor
{
    CENTER,
    LEFT,
    RIGHT,
    TOP,
    BOTTOM,
    LEFT_TOP,
    LEFT_BOTTOM,
    RIGHT_TOP,
    RIGHT_BOTTOM
}
public enum Display_Surround
{
    NONE,
    DISPLAY_3
}
public enum GameResult
{
    NONE,
    FINISH,
    RETIRE
}


[System.Serializable]
public class CircleOnOffImage
{
    public Sprite OnImage;
    public Sprite OffImage;
}

[System.Serializable]
public class MapSelectInfo
{
    public string mMapName;
    public string mDifficulty;
}

public enum TimeMode
{
    None,
    Time,
    GrandPrix
}

[System.Serializable]
public class LoadMapData
{
    public TimeMode timeMode;
    public int map;
    public int car;
}