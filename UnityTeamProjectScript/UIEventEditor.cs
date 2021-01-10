using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using UnityEngine.UI;

[CustomEditor(typeof(UIEvent))]
public class UIEventEditor : Editor
{
    UIEvent _editor;
    UIEventList eventList;
    GameObject tmp = null;

    private void OnEnable()
    {
        _editor = target as UIEvent;
    }


    public override void OnInspectorGUI()
    {
        EditorGUI.BeginChangeCheck();
        eventList = (UIEventList)EditorGUILayout.EnumPopup("EventList", _editor.eventList);
        if(EditorGUI.EndChangeCheck())
        {
            Undo.RecordObject(_editor, "Event Change");
            _editor.eventList = eventList;
        }

        CheckList();
    }

    void CheckList()
    {
        switch(_editor.eventList)
        {
            case UIEventList.OpenPanel:
                EditorGUI.BeginChangeCheck();
                tmp = (GameObject)EditorGUILayout.ObjectField("패널", _editor.panel, typeof(GameObject), true);
                if (EditorGUI.EndChangeCheck())
                {
                    Undo.RecordObject(_editor, "Panel Change");
                    _editor.panel = tmp;
                }
                break;


            case UIEventList.ClosePanel:
                EditorGUI.BeginChangeCheck();
                tmp = (GameObject)EditorGUILayout.ObjectField("패널", _editor.panel, typeof(GameObject), true);
                if (EditorGUI.EndChangeCheck())
                {
                    Undo.RecordObject(_editor, "Panel Change");
                    _editor.panel = tmp;
                }
                break;

            case UIEventList.NoneClosePanel:
                EditorGUI.BeginChangeCheck();
                tmp = (GameObject)EditorGUILayout.ObjectField("패널", _editor.panel, typeof(GameObject), true);
                if (EditorGUI.EndChangeCheck())
                {
                    Undo.RecordObject(_editor, "Panel Change");
                    _editor.panel = tmp;
                }
                break;

            case UIEventList.NoneOpenPanel:
                EditorGUI.BeginChangeCheck();
                tmp = (GameObject)EditorGUILayout.ObjectField("패널", _editor.panel, typeof(GameObject), true);
                if (EditorGUI.EndChangeCheck())
                {
                    Undo.RecordObject(_editor, "Panel Change");
                    _editor.panel = tmp;
                }
                break;


            case UIEventList.NextScenes:
                EditorGUI.BeginChangeCheck();
                var name = (string)EditorGUILayout.TextField("씬 이름", _editor.ScenesName);
                if (EditorGUI.EndChangeCheck())
                {
                    Undo.RecordObject(_editor, "SceneName Change");
                    _editor.ScenesName = name;
                }
                break;

            case UIEventList.OutClient:
                break;


            case UIEventList.OpenMenu:
                EditorGUI.BeginChangeCheck();
                tmp = (GameObject)EditorGUILayout.ObjectField("시작", _editor.panel, typeof(GameObject), true);
                if (EditorGUI.EndChangeCheck())
                {
                    Undo.RecordObject(_editor, "Panel Change");
                    _editor.panel = tmp;
                }


                EditorGUI.BeginChangeCheck();
                tmp = (GameObject)EditorGUILayout.ObjectField("종료", _editor.menu, typeof(GameObject), true);
                if (EditorGUI.EndChangeCheck())
                {
                    Undo.RecordObject(_editor, "Panel Change");
                    _editor.menu = tmp;
                }
                break;

            case UIEventList.CloseMenu:
                EditorGUI.BeginChangeCheck();
                tmp = (GameObject)EditorGUILayout.ObjectField("종료", _editor.panel, typeof(GameObject), true);
                if (EditorGUI.EndChangeCheck())
                {
                    Undo.RecordObject(_editor, "Panel Change");
                    _editor.panel = tmp;
                }


                EditorGUI.BeginChangeCheck();
                tmp = (GameObject)EditorGUILayout.ObjectField("시작", _editor.menu, typeof(GameObject), true);
                if (EditorGUI.EndChangeCheck())
                {
                    Undo.RecordObject(_editor, "Panel Change");
                    _editor.menu = tmp;
                }
                break;

            case UIEventList.MessageBoxBackMenu:
                EditorGUI.BeginChangeCheck();
                tmp = (GameObject)EditorGUILayout.ObjectField("종료", _editor.panel, typeof(GameObject), true);
                if (EditorGUI.EndChangeCheck())
                {
                    Undo.RecordObject(_editor, "Panel Change");
                    _editor.panel = tmp;
                }


                EditorGUI.BeginChangeCheck();
                tmp = (GameObject)EditorGUILayout.ObjectField("시작", _editor.menu, typeof(GameObject), true);
                if (EditorGUI.EndChangeCheck())
                {
                    Undo.RecordObject(_editor, "Panel Change");
                    _editor.menu = tmp;
                }

                EditorGUI.BeginChangeCheck();
                tmp = (GameObject)EditorGUILayout.ObjectField("메시지박스", _editor.messagebox, typeof(GameObject), true);
                if (EditorGUI.EndChangeCheck())
                {
                    Undo.RecordObject(_editor, "Panel Change");
                    _editor.messagebox = tmp;
                }
                break;

            case UIEventList.MapSelect:
                EditorGUI.BeginChangeCheck();
                var sname = (string)EditorGUILayout.TextField("씬 이름", _editor.ScenesName);
                if (EditorGUI.EndChangeCheck())
                {
                    Undo.RecordObject(_editor, "SceneName Change");
                    _editor.ScenesName = sname;
                }
                break;

            case UIEventList.GameRestart:
                EditorGUI.BeginChangeCheck();
                var ssname = (string)EditorGUILayout.TextField("씬 이름", _editor.ScenesName);
                if (EditorGUI.EndChangeCheck())
                {
                    Undo.RecordObject(_editor, "SceneName Change");
                    _editor.ScenesName = ssname;
                }
                break;

            case UIEventList.InputName:

                EditorGUI.BeginChangeCheck();
                tmp = (GameObject)EditorGUILayout.ObjectField("Close패널", _editor.panel, typeof(GameObject), true);
                if (EditorGUI.EndChangeCheck())
                {
                    Undo.RecordObject(_editor, "Panel Change");
                    _editor.panel = tmp;
                }

                Text tmpText;
                EditorGUI.BeginChangeCheck();
                tmpText = (Text)EditorGUILayout.ObjectField("Text", _editor.text, typeof(Text), true);
                if (EditorGUI.EndChangeCheck())
                {
                    Undo.RecordObject(_editor, "Panel Change");
                    _editor.text = tmpText;
                }
                break;

            case UIEventList.OpenAndSetActivity:
                EditorGUI.BeginChangeCheck();
                tmp = (GameObject)EditorGUILayout.ObjectField("종료", _editor.panel, typeof(GameObject), true);
                if (EditorGUI.EndChangeCheck())
                {
                    Undo.RecordObject(_editor, "Panel Change");
                    _editor.panel = tmp;
                }


                EditorGUI.BeginChangeCheck();
                tmp = (GameObject)EditorGUILayout.ObjectField("시작", _editor.menu, typeof(GameObject), true);
                if (EditorGUI.EndChangeCheck())
                {
                    Undo.RecordObject(_editor, "Panel Change");
                    _editor.menu = tmp;
                }
                break;

            case UIEventList.CloseAndSetActivity:
                EditorGUI.BeginChangeCheck();
                tmp = (GameObject)EditorGUILayout.ObjectField("종료", _editor.panel, typeof(GameObject), true);
                if (EditorGUI.EndChangeCheck())
                {
                    Undo.RecordObject(_editor, "Panel Change");
                    _editor.panel = tmp;
                }


                EditorGUI.BeginChangeCheck();
                tmp = (GameObject)EditorGUILayout.ObjectField("시작", _editor.menu, typeof(GameObject), true);
                if (EditorGUI.EndChangeCheck())
                {
                    Undo.RecordObject(_editor, "Panel Change");
                    _editor.menu = tmp;
                }
                break;
        }
    }
}
