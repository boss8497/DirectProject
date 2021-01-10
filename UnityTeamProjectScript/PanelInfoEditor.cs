using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using UnityEngine.UI;

[CustomEditor(typeof(PanelInfo))]
public class PanelInfoEditor : Editor
{
    PanelInfo _editor;
    PanelMode mode;
    int menuIndex;
    int submenuIndex;
    SerializedProperty escEvent;
    SerializedProperty conoff;

    private void OnEnable()
    {
        _editor = target as PanelInfo;
        escEvent = serializedObject.FindProperty("EscEvent");
        conoff = serializedObject.FindProperty("circleonoff");
    }

    public override void OnInspectorGUI()
    {
        //원본 인스펙터
        //base.OnInspectorGUI();

        EditorGUI.BeginChangeCheck();
        mode = (PanelMode)EditorGUILayout.EnumPopup("메뉴 타입", _editor.mode);
        if (EditorGUI.EndChangeCheck())
        {
            Undo.RecordObject(_editor, "Enum AnchorSelect");
            _editor.mode = mode;
        }

        CheckEnumField();

        serializedObject.Update();
        EditorGUILayout.PropertyField(escEvent);

        if (serializedObject.ApplyModifiedProperties())
        {
            if (_editor.EscEvent)
            {
                serializedObject.ApplyModifiedProperties();
            }
        }
    }


    /// <summary>
    /// Method Field
    /// </summary>

    void CheckEnumField()
    {
        switch (_editor.mode)
        {
            case PanelMode.NONE:
                {
                    MenuCountInspector();
                    ShowMenuInspector();
                }
                break;

            case PanelMode.MESSAGEBOX:
                {
                    MenuCountInspector();
                    ShowMenuInspector();


                    serializedObject.Update();
                    EditorGUILayout.PropertyField(conoff);
                    serializedObject.ApplyModifiedProperties();
                }
                break;

            case PanelMode.VIEW_SCROLL:
                {
                    ModeSelectInspector();
                    MenuCountInspector();
                    SubMenuCountInspector();
                    ShowMenuInspector();
                    ShowSubMenuInspector();
                    ContentInspector();
                }
                break;

            case PanelMode.Circle:
                {
                    LogoInspector();
                    if (MenuCountInspector())
                    {
                        if (_editor.menuIndex > 0)
                            CreateCircleMenu();
                    }
                    ShowMenuInspector();
                    ContentInspector();




                    serializedObject.Update();
                    EditorGUILayout.PropertyField(conoff);
                    serializedObject.ApplyModifiedProperties();
                }
                break;

            case PanelMode.CAR_SELECT:
                {
                    CarNameInspector();
                    MenuCountInspector();
                    ShowMenuInspector();
                    CarContentInspector();

                    CarCircleInspector();
                    CarCountInspector();
                    ShowCarInspector();
                    CarBackGroundInspector();
                }
                break;

            case PanelMode.GAME_RESULT:
                {

                    if (MenuCountInspector())
                    {
                        if (_editor.menuIndex > 0)
                            CreateCircleMenu();
                    }
                    ShowMenuInspector();
                    ContentInspector();


                    if (SubMenuCountInspector())
                    {
                        if (_editor.submenuIndex > 0)
                            CreateCircleSubMenu();
                    }
                    ShowSubMenuInspector();
                    SubContentInspector();


                    serializedObject.Update();
                    EditorGUILayout.PropertyField(conoff);
                    serializedObject.ApplyModifiedProperties();
                }
                break;
        }
    }

    void LogoInspector()
    {
        GameObject rf;
        EditorGUI.BeginChangeCheck();
        rf = (GameObject)EditorGUILayout.ObjectField("Logo", _editor.Logo, typeof(GameObject), true);

        if (EditorGUI.EndChangeCheck())
        {
            Undo.RecordObject(_editor, "Editor");
            _editor.Logo = rf;
        }
    }

    void ContentInspector()
    {
        RectTransform rf;
        EditorGUI.BeginChangeCheck();
        rf = (RectTransform)EditorGUILayout.ObjectField("Content", _editor.content, typeof(RectTransform), true);

        if (EditorGUI.EndChangeCheck())
        {
            Undo.RecordObject(_editor, "Editor");
            _editor.content = rf;
        }
    }

    void CarContentInspector()
    {
        Transform rf;
        EditorGUI.BeginChangeCheck();
        rf = (Transform)EditorGUILayout.ObjectField("CarContent", _editor.CarContent, typeof(Transform), true);

        if (EditorGUI.EndChangeCheck())
        {
            Undo.RecordObject(_editor, "Editor");
            _editor.CarContent = rf;
        }
    }

    void CarCircleInspector()
    {
        Transform rf;
        EditorGUI.BeginChangeCheck();
        rf = (Transform)EditorGUILayout.ObjectField("CarCircle", _editor.CarCircle, typeof(Transform), true);

        if (EditorGUI.EndChangeCheck())
        {
            Undo.RecordObject(_editor, "Editor");
            _editor.CarCircle = rf;
        }
    }

    void CarBackGroundInspector()
    {
        GameObject rf;
        EditorGUI.BeginChangeCheck();
        rf = (GameObject)EditorGUILayout.ObjectField("CarBackGround", _editor.CarBackGround, typeof(GameObject), true);

        if (EditorGUI.EndChangeCheck())
        {
            Undo.RecordObject(_editor, "Editor");
            _editor.CarBackGround = rf;
        }
    }

    void SubContentInspector()
    {
        RectTransform rf;
        EditorGUI.BeginChangeCheck();
        rf = (RectTransform)EditorGUILayout.ObjectField("Content", _editor.subcontent, typeof(RectTransform), true);

        if (EditorGUI.EndChangeCheck())
        {
            Undo.RecordObject(_editor, "Editor");
            _editor.subcontent = rf;
        }
    }

    void CarNameInspector()
    {
        Text rf;
        EditorGUI.BeginChangeCheck();
        rf = (Text)EditorGUILayout.ObjectField("차이름 텍스트", _editor.CarName, typeof(Text), true);

        if (EditorGUI.EndChangeCheck())
        {
            Undo.RecordObject(_editor, "Editor");
            _editor.CarName = rf;
        }
    }

    void ModeSelectInspector()
    {
        PanelInfo rf;
        EditorGUI.BeginChangeCheck();
        rf = (PanelInfo)EditorGUILayout.ObjectField("ModeSelectPanel", _editor.ModeSelectPanel, typeof(PanelInfo), true);

        if (EditorGUI.EndChangeCheck())
        {
            Undo.RecordObject(_editor, "Editor");
            _editor.ModeSelectPanel = rf;
        }
    }

    bool MenuCountInspector()
    {
        EditorGUI.BeginChangeCheck();
        menuIndex = (int)EditorGUILayout.IntField("메뉴 개수", _editor.menuIndex);
        if (EditorGUI.EndChangeCheck())
        {
            Undo.RecordObject(_editor, "Enum AnchorSelect");
            _editor.menuIndex = menuIndex;
            
            _editor.Menu = new RectTransform[_editor.menuIndex];
            return true;
        }

        return false;
    }

    bool CarCountInspector()
    {
        EditorGUI.BeginChangeCheck();
        int CarCount = (int)EditorGUILayout.IntField("메뉴 개수", _editor.CarCount);
        if (EditorGUI.EndChangeCheck())
        {
            Undo.RecordObject(_editor, "Enum AnchorSelect");
            _editor.CarCount = CarCount;

            _editor.CarSelect = new Transform[_editor.CarCount];
            return true;
        }

        return false;
    }

    bool SubMenuCountInspector()
    {
        EditorGUI.BeginChangeCheck();
        submenuIndex = (int)EditorGUILayout.IntField("서브메뉴 개수", _editor.submenuIndex);
        if (EditorGUI.EndChangeCheck())
        {
            Undo.RecordObject(_editor, "Enum AnchorSelect");
            _editor.submenuIndex = submenuIndex;

            _editor.SubMenu = new RectTransform[_editor.submenuIndex];
            return true;
        }

        return false;
    }

    void ShowMenuInspector()
    {
        if (_editor.menuIndex > 0)
        {
            for (int i = 0; i < _editor.menuIndex; i++)
            {

                RectTransform rf;
                EditorGUI.BeginChangeCheck();

                rf = (RectTransform)EditorGUILayout.ObjectField(string.Format("메뉴 {0}", i + 1), _editor.Menu[i], typeof(RectTransform), true);

                if (EditorGUI.EndChangeCheck())
                {
                    Undo.RecordObject(_editor, "Editor");
                    _editor.Menu[i] = rf;
                }
            }
        }
    }

    void ShowCarInspector()
    {
        if (_editor.CarCount > 0)
        {
            for (int i = 0; i < _editor.CarCount; i++)
            {

                Transform rf;
                EditorGUI.BeginChangeCheck();

                rf = (Transform)EditorGUILayout.ObjectField(string.Format("메뉴 {0}", i + 1), _editor.CarSelect[i], typeof(Transform), true);

                if (EditorGUI.EndChangeCheck())
                {
                    Undo.RecordObject(_editor, "Editor");
                    _editor.CarSelect[i] = rf;
                }
            }
        }
    }


    void ShowSubMenuInspector()
    {
        if (_editor.submenuIndex > 0)
        {
            for (int i = 0; i < _editor.submenuIndex; i++)
            {

                RectTransform rf;
                EditorGUI.BeginChangeCheck();

                rf = (RectTransform)EditorGUILayout.ObjectField(string.Format("서브메뉴 {0}", i + 1), _editor.SubMenu[i], typeof(RectTransform), true);

                if (EditorGUI.EndChangeCheck())
                {
                    Undo.RecordObject(_editor, "Editor");
                    _editor.SubMenu[i] = rf;
                }
            }
        }
    }

    void CreateCircleMenu()
    {
        int circleDgree = 360 / _editor.menuIndex;
        UIPosition uipos;
        RectTransform tmprect;
        Image img;
        GameObject panel = new GameObject("CircleMenu");
        tmprect = panel.AddComponent<RectTransform>();
        img = panel.AddComponent<Image>();
        uipos = panel.AddComponent<UIPosition>();

        uipos.ScreenSize = new Vector2(1920, 1080);
        uipos.anchor = Anchor.CENTER;
        uipos.Circle = true;
        uipos.Div_y = uipos.Div_x = 10;
        uipos.Pos_x = uipos.Pos_y = 0;
        uipos.Size_x = uipos.Size_y = 5f;

        img.color = new Vector4(255, 255, 255, 0);
        img.raycastTarget = false;
        tmprect.SetParent(_editor.transform);
        tmprect.localPosition = Vector3.zero;
        _editor.content = tmprect;

        uipos.EditorSetPosSize();

        for (int i = 0; i < _editor.menuIndex; i++)
        {
            _editor.Menu[i] = CreateSectorform(tmprect, circleDgree);
        }

        int startDgree = 0;

        for (int i = 0; i < _editor.menuIndex; i++)
        {
            int nowDgree = startDgree + (circleDgree * -i);
            _editor.Menu[i].Rotate(new Vector3(0, 0, nowDgree));
        }

    }


    void CreateCircleSubMenu()
    {
        int circleDgree = 360 / _editor.submenuIndex;
        UIPosition uipos;
        RectTransform tmprect;
        Image img;
        GameObject panel = new GameObject("CircleMenu");
        tmprect = panel.AddComponent<RectTransform>();
        img = panel.AddComponent<Image>();
        uipos = panel.AddComponent<UIPosition>();

        uipos.ScreenSize = new Vector2(1920, 1080);
        uipos.anchor = Anchor.CENTER;
        uipos.Circle = true;
        uipos.Div_y = uipos.Div_x = 10;
        uipos.Pos_x = uipos.Pos_y = 0;
        uipos.Size_x = uipos.Size_y = 5f;

        img.color = new Vector4(255, 255, 255, 0);
        img.raycastTarget = false;
        tmprect.SetParent(_editor.transform);
        tmprect.localPosition = Vector3.zero;
        _editor.subcontent = tmprect;

        uipos.EditorSetPosSize();

        for (int i = 0; i < _editor.submenuIndex; i++)
        {
            _editor.SubMenu[i] = CreateSectorform(tmprect, circleDgree);
        }

        int startDgree = 0;

        for (int i = 0; i < _editor.submenuIndex; i++)
        {
            int nowDgree = startDgree + (circleDgree * -i);
            _editor.SubMenu[i].Rotate(new Vector3(0, 0, nowDgree));
        }

    }

    RectTransform CreateSectorform(RectTransform parent, int dgree)
    {
        GameObject gm = new GameObject("Sector");
        UIPosition uipos;
        RectTransform rf = null;
        Sectorform sf;

        rf = gm.AddComponent<RectTransform>();
        sf = gm.AddComponent<Sectorform>();
        uipos = gm.AddComponent<UIPosition>();

        uipos.anchor = Anchor.CENTER;
        uipos.Local = true;
        uipos.Div_y = uipos.Div_x = 1;
        uipos.Pos_x = uipos.Pos_y = 0;
        uipos.Size_x = uipos.Size_y = 1;

        sf.color = new Color(Random.Range(0.0f, 1.0f), Random.Range(0.0f, 1.0f), Random.Range(0.0f, 1.0f), 1.0f);
        sf.segments = 320;
        sf.Degree = dgree;

        //rf.sizeDelta = parent.sizeDelta;
        //rf.position = parent.position;

        rf.SetParent(parent.transform);
        uipos.SetParent();
        uipos.EditorSetPosSize();

        return rf;
    }
}


[CustomPropertyDrawer(typeof(CircleOnOffImage))]
public class CircleOnOffDrawe : PropertyDrawer
{
    public override void OnGUI(Rect position, SerializedProperty property, GUIContent label)
    {
        using (new EditorGUI.PropertyScope(position, label, property))
        {
            var OnProperty = property.FindPropertyRelative("OnImage");
            var OffProperty = property.FindPropertyRelative("OffImage");



            EditorGUILayout.PropertyField(OnProperty);
            EditorGUILayout.PropertyField(OffProperty);
        }
    }
}

[CustomPropertyDrawer(typeof(MapSelectInfo))]
public class MapInfoDrawe : PropertyDrawer
{
    public override void OnGUI(Rect position, SerializedProperty property, GUIContent label)
    {
        using (new EditorGUI.PropertyScope(position, label, property))
        {
            var mapname = property.FindPropertyRelative("mMapName");
            var difficul = property.FindPropertyRelative("mDifficulty");
            
            EditorGUILayout.PropertyField(mapname);
            EditorGUILayout.PropertyField(difficul);
        }
    }
}