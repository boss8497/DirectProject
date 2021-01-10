using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using UnityEngine.UI;

[CustomEditor(typeof(UIPosition))]
public class UIPositionEditor : Editor
{
    UIPosition _editor;
    Vector2 ScreenSize;
    float changeFloat;
    SerializedProperty local,circle,istext;
    Anchor anchor;
    Text text;

    private void OnEnable()
    {
        _editor = target as UIPosition;
        local = serializedObject.FindProperty("Local");
        circle = serializedObject.FindProperty("Circle");
        istext = serializedObject.FindProperty("IsText");
    }


    public override void OnInspectorGUI()
    {

        EditorGUILayout.BeginHorizontal();

        serializedObject.Update();
        EditorGUILayout.PropertyField(local);

        if (serializedObject.ApplyModifiedProperties())
        {
            if (_editor.Local)
            {
                _editor.SetParent();
                serializedObject.ApplyModifiedProperties();
            }
        }

        serializedObject.Update();
        EditorGUILayout.PropertyField(circle);

        if (serializedObject.ApplyModifiedProperties())
        {
            if (_editor.Circle)
            {
                _editor.SetParent();
                serializedObject.ApplyModifiedProperties();
            }
        }

        EditorGUILayout.EndHorizontal();


        EditorGUI.BeginChangeCheck();
        anchor = (Anchor)EditorGUILayout.EnumPopup("중심점 위치(anchor)", _editor.anchor);
        if (EditorGUI.EndChangeCheck())
        {
            Undo.RecordObject(_editor, "Enum AnchorSelect");
            _editor.anchor = anchor;
        }
        _editor.SetAnchors();

        EditorGUI.BeginChangeCheck();
        ScreenSize = (Vector2)EditorGUILayout.Vector2Field("해상도", _editor.ScreenSize);
        if(EditorGUI.EndChangeCheck())
        {
            Undo.RecordObject(_editor, "ScreenSize");
            _editor.ScreenSize = ScreenSize;
        }
        

        EditorGUILayout.BeginHorizontal();

        EditorGUI.BeginChangeCheck();
        changeFloat = (float)EditorGUILayout.FloatField("분할크기X ( " + _editor.ScreenSize.x + "/x )", _editor.Div_x);
        if (EditorGUI.EndChangeCheck())
        {
            Undo.RecordObject(_editor, "Div_X");
            _editor.Div_x = changeFloat;
        }

        EditorGUI.BeginChangeCheck();
        changeFloat = (float)EditorGUILayout.FloatField("분할크기Y ( " + _editor.ScreenSize.y + "/y )", _editor.Div_y);
        if (EditorGUI.EndChangeCheck())
        {
            Undo.RecordObject(_editor, "Div_Y");
            _editor.Div_y = changeFloat;
        }
        EditorGUILayout.EndHorizontal();


        EditorGUILayout.BeginHorizontal();
        
        EditorGUI.BeginChangeCheck();
        changeFloat = (float)EditorGUILayout.FloatField("Xpos = " +  (_editor.ScreenSize.x/_editor.Div_x) + " * value (위치)", _editor.Pos_x);
        if (EditorGUI.EndChangeCheck())
        {
            Undo.RecordObject(_editor, "X_Pos");
            _editor.Pos_x = changeFloat;
        }
        
        EditorGUI.BeginChangeCheck();
        changeFloat = (float)EditorGUILayout.FloatField("Ypos = " + (_editor.ScreenSize.y/_editor.Div_y) + " * value  (위치)", _editor.Pos_y);
        if (EditorGUI.EndChangeCheck())
        {
            Undo.RecordObject(_editor, "Y_Pos");
            _editor.Pos_y = changeFloat;
        }

        EditorGUILayout.EndHorizontal();



        EditorGUILayout.BeginHorizontal();

        EditorGUI.BeginChangeCheck();
        changeFloat = (float)EditorGUILayout.FloatField("Xsize = "+ (_editor.ScreenSize.x / _editor.Div_x) + " * value (크기)", _editor.Size_x);
        if (EditorGUI.EndChangeCheck())
        {
            Undo.RecordObject(_editor, "X_Size");
            _editor.Size_x = changeFloat;
        }

        EditorGUI.BeginChangeCheck();
        changeFloat = (float)EditorGUILayout.FloatField("Ysize = " + (_editor.ScreenSize.y / _editor.Div_y) + " * value (크기)", _editor.Size_y);
        if (EditorGUI.EndChangeCheck())
        {
            Undo.RecordObject(_editor, "Y_Size");
            _editor.Size_y = changeFloat;
        }

        EditorGUILayout.EndHorizontal();


        serializedObject.Update();
        EditorGUILayout.PropertyField(istext);

        if (serializedObject.ApplyModifiedProperties())
        {
            if (_editor.IsText)
            {
                _editor.SetParent();
                serializedObject.ApplyModifiedProperties();
            }
        }

        if(_editor.IsText)
        {

            EditorGUI.BeginChangeCheck();

            text = (Text)EditorGUILayout.ObjectField("텍스트", _editor.text, typeof(Text), true);

            if (EditorGUI.EndChangeCheck())
            {
                Undo.RecordObject(_editor, "Editor");
                _editor.text = text;
            }

            EditorGUI.BeginChangeCheck();
            changeFloat = (float)EditorGUILayout.FloatField("원본 크기", _editor.BaseFontSize);
            if (EditorGUI.EndChangeCheck())
            {
                Undo.RecordObject(_editor, "Editor");
                _editor.BaseFontSize = changeFloat;
            }
            
        }

        _editor.EditorSetPosSize();
    }

    
   
}
