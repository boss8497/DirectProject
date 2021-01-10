using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using LitJson;


public class UI_StringManager : Singleton<UI_StringManager>
{


    /*타이틀 메뉴*/
    public Text TitleMenuName;                  //타이틀 메뉴의 이름 Text컴포넌트
    public Text TitleMenuContent;               //타이틀 메뉴의 설명 Text컴포넌트
    public TitleMenuString[] titleMenuStrings;  //타이틀 메뉴에 적용할 string

    /*타이틀 모드셀렉 메뉴*/
    public TitleMenuString[] titleModeMenuStrings;


    /*차량 스텟*/
    //public Image CarLogo;
    public Text CarNameText;
    public Text CarCompanyText;
    public Image[] StatView;
    public Text[] NumberStatView;
    [HideInInspector]
    public CarStat[] carStat;






    /*맵 기록*/
    public Text[] RecordNameText;
    public Text[] RecordText;



    /*맵기록 Nodata*/
    public string NoRecordName = "";
    public string NoRecord = "00:00:00";



    /**/
    float StatChangeSpeed = 0f;
    int NowIndex = 0;




    /*타이틀 메뉴*/
    public void ChangeTitleString(int index)
    {
        if (titleMenuStrings.Length - 1 < index)
        {
            return;
        }


        TitleMenuName.text = titleMenuStrings[index].MenuName;
        TitleMenuContent.text = titleMenuStrings[index].MenuContent;
    }
    public void ChangeTitleModeString(int index)
    {
        if (titleModeMenuStrings.Length - 1 < index)
        {
            return;
        }


        TitleMenuName.text = titleModeMenuStrings[index].MenuName;
        TitleMenuContent.text = titleModeMenuStrings[index].MenuContent;
    }



    /*차량 스텟*/
    public void SetCarData()
    {
        int size = LoadCarStatusData.instance.m_cars.Length;
        carStat = new CarStat[size];
        

        for (int i = 0; i < size; ++i)
        {
            carStat[i].mCarModel = LoadCarStatusData.instance.m_cars[i].m_selectSceneModel;
            carStat[i].mCarLogo = LoadCarStatusData.instance.m_cars[i].m_carBackGroundSprite;
            carStat[i].mMaxDurability = LoadCarStatusData.instance.m_cars[i].m_durabilityLevel;
            carStat[i].mSubDurability = LoadCarStatusData.instance.m_cars[i].m_durabilityAmountLevel;
            carStat[i].mCarName = LoadCarStatusData.instance.m_cars[i].m_carName;
            carStat[i].mCarCompany = LoadCarStatusData.instance.m_cars[i].m_carDescription;
            carStat[i].mMaxSpeed = LoadCarStatusData.instance.m_cars[i].m_maxSpeedLevel;
            carStat[i].mAcceleration = LoadCarStatusData.instance.m_cars[i].m_accelLevel;
            carStat[i].mCornering = LoadCarStatusData.instance.m_cars[i].m_corneringLevel;
            carStat[i].mStability = LoadCarStatusData.instance.m_cars[i].m_comLevel;
            carStat[i].mLogoPosition = LoadCarStatusData.instance.m_cars[i].m_carLogoPosition;
            carStat[i].mLogoSize = LoadCarStatusData.instance.m_cars[i].m_carLogoSize;
            carStat[i].mLogoRotate = LoadCarStatusData.instance.m_cars[i].m_carLogoRotate;
        }
    }
    public void ChangeCarStat(int index, float aniSpeed)
    {
        StatChangeSpeed = aniSpeed;
        NowIndex = index;

        StopAllCoroutines();
        for (int i = 0; i < 6; i++)
        {
            StartCoroutine("StatAnimation", i);
        }
        CarNameText.text = carStat[index].mCarName;
        CarCompanyText.text = carStat[index].mCarCompany;
        //CarLogo.sprite = carStat[index].mCarLogo;
    }


    /*맵선택 레코드 변경*/
    public void ChangeRecord(JsonData data)
    {
        if (data.Count == 0)
        {
            //Debug.Log("No JsonData");
        }
        StopAllCoroutines();
        StartCoroutine("RecordAnimation", data);

    }





    /*Index 설정*/
    const int MaxDurability = 0; //차량 내구도
    const int SubDurability = 1; //내구도 소모
    const int MaxSpeed = 2;      //최대 속도
    const int Acceleration = 3;  //가속도
    const int Cornering = 4;     //코너링
    const int Stability = 5;     //안정감
    const int CarName = 6;       //차이름








    IEnumerator StatAnimation(int index)
    {
        float target = 0f;

        switch (index)
        {
            case MaxDurability:
                target = carStat[NowIndex].mMaxDurability * 0.1f;
                /*2가지 방법*/
                //NumberStatView[index].text = string.Format("{0:0.0}", carStat[NowIndex].mMaxDurability);
                NumberStatView[index].text = carStat[NowIndex].mMaxDurability.ToString("N1");
                break;

            case SubDurability:
                target = carStat[NowIndex].mSubDurability * 0.1f;
                NumberStatView[index].text = string.Format("{0:0.0}", carStat[NowIndex].mSubDurability);
                break;

            case MaxSpeed:
                target = carStat[NowIndex].mMaxSpeed * 0.1f;
                NumberStatView[index].text = string.Format("{0:0.0}", carStat[NowIndex].mMaxSpeed);
                break;

            case Acceleration:
                target = carStat[NowIndex].mAcceleration * 0.1f;
                NumberStatView[index].text = string.Format("{0:0.0}", carStat[NowIndex].mAcceleration);
                break;

            case Cornering:
                target = carStat[NowIndex].mCornering * 0.1f;
                NumberStatView[index].text = string.Format("{0:0.0}", carStat[NowIndex].mCornering);
                break;

            case Stability:
                target = carStat[NowIndex].mStability * 0.1f;
                NumberStatView[index].text = string.Format("{0:0.0}", carStat[NowIndex].mStability);
                break;
        }


        float t = 0f;

        StatView[index].fillAmount = 0f;
        while (true)
        {
            float result = Mathf.Lerp(StatView[index].fillAmount, target, t);

            StatView[index].fillAmount = result;

            t += Time.deltaTime * StatChangeSpeed;


            if (StatView[index].fillAmount == target)
                break;

            yield return null;
        }
    }

    IEnumerator RecordAnimation(JsonData data)
    {
        string[] name = new string[data.Count];
        string[] record = new string[data.Count];

        for (int i = 0; i < RecordNameText.Length; i++)
        {
            RecordNameText[i].text = "";
        }

        for (int i = 0; i < RecordText.Length; i++)
        {
            RecordText[i].text = "";
        }

        for (int j = 0; j < data.Count; j++)
        {
            name[j] = data[j]["PlayerName"].ToString();
            string str = "";

            if (int.Parse(data[j]["Min"].ToString()) < 10)
                str = " " + data[j]["Min"].ToString();
            else
                str = data[j]["Min"].ToString();

            str += ":";

            if (int.Parse(data[j]["Sec"].ToString()) < 10)
                str += "0" + data[j]["Sec"].ToString();
            else
                str += data[j]["Sec"].ToString();
            str += ":";


            if (int.Parse(data[j]["mSec"].ToString()) < 10)
                str += "0" + data[j]["mSec"].ToString();
            else
                str += data[j]["mSec"].ToString();

            record[j] = str;
        }


        int maxname = 0;
        foreach (var m in name)
        {
            if (maxname < m.Length)
            {
                maxname = m.Length;
            }
        }


        //바꾼코드
        for (int j = 0; j < name.Length; j++)
        {
            RecordNameText[j].text += name[j].ToString();
            yield return null;
        }


        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < RecordText.Length; j++)
            {
                if (j < record.Length)
                {
                    if (record[j].Length > i)
                        RecordText[j].text += record[j][i].ToString();
                }
                else
                {
                    switch (i)
                    {
                        case 0:
                            RecordText[j].text = "0";
                            break;
                        case 1:
                            RecordText[j].text += "0";
                            break;
                        case 2:
                            RecordText[j].text += ":";
                            break;
                        case 3:
                            RecordText[j].text += "0";
                            break;
                        case 4:
                            RecordText[j].text += "0";
                            break;
                        case 5:
                            RecordText[j].text += ":";
                            break;
                        case 6:
                            RecordText[j].text += "0";
                            break;
                        case 7:
                            RecordText[j].text += "0";
                            break;
                    }
                }
            }
            yield return new WaitForSeconds(0.08f);
        }
    }
}


[System.Serializable]
public class TitleMenuString
{
    [TextArea]
    public string MenuName;     //메뉴 이름
    [TextArea]
    public string MenuContent;  //메뉴 설명
}


[System.Serializable]
public struct CarStat
{
    public GameObject mCarModel;
    public Sprite mCarLogo;
    public Vector3 mLogoPosition;
    public Vector2 mLogoSize;
    public Vector3 mLogoRotate;
    public float mMaxDurability; //차량 내구도
    public float mSubDurability; //내구도 소모
    public float mMaxSpeed;      //최대 속도
    public float mAcceleration;  //가속도
    public float mCornering;     //코너링
    public float mStability;     //안정감
    public string mCarName;    //차이름
    public string mCarCompany; //차회사
}

