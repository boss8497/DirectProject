using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UI_SoundManager : Singleton<UI_SoundManager>
{
    AudioSource audio;
    AudioSource BackGoundMusic;
    [Range(0f, 1.0f)]
    public float UI_Effect_Volume = 1.0f;

    public AudioClip EscSound;
    public AudioClip MessageBoxMoveSound;
    public AudioClip MessageBoxSelectSound;


    public AudioClip CircleMenuMoveSound;
    public AudioClip CircleMenuSelectSound;

    public AudioClip MapSelectMoveSound;
    public AudioClip MapSelectSound;


    public AudioClip CarSelectMoveSound;
    public AudioClip CarSelectSound;


    [Range(0f,1.0f)]
    public float BackGroundMusicVolume = 1.0f;
    public AudioClip OneshotTitleBackGroundMusic;
    public AudioClip TitleBackGroundMusic;
    public AudioClip MapSelectBackGroundMusic;
    public AudioClip CarSelectBackGroundMusic;

    UI_BackGoundMusic Type = UI_BackGoundMusic.None;


    bool IsTitleOnshot = false;

    private void Awake()
    {
        audio = GetComponent<AudioSource>();
        BackGoundMusic = gameObject.AddComponent<AudioSource>();
        BackGoundMusic.loop = true;
        BackGoundMusic.priority = 0;
        BackGoundMusic.volume = BackGroundMusicVolume;
        audio.volume = UI_Effect_Volume;


    }
    


    public void OneShotPlaySound(SoundMode mode)
    {
        audio.volume = UI_Effect_Volume;
        audio.Stop();
        switch (mode)
        {
            case SoundMode.Esc:
                if (EscSound)
                {
                    audio.PlayOneShot(EscSound);
                }
                else
                {
                    Debug.LogErrorFormat("EscSound NullPointer");
                }
                break;
            case SoundMode.MesaageBoxMove:
                if (MessageBoxMoveSound)
                {
                    audio.PlayOneShot(MessageBoxMoveSound);
                }
                else
                {
                    Debug.LogErrorFormat("MessageBoxMoveSound NullPointer");
                }
                break;
            case SoundMode.MessageBoxSelect:
                if (MessageBoxSelectSound)
                {
                    audio.PlayOneShot(MessageBoxSelectSound);
                }
                else
                {
                    Debug.LogErrorFormat("MessageBoxSelectSound NullPointer");
                }
                break;
            case SoundMode.CircleMenuMove:
                if (CircleMenuMoveSound)
                {
                    audio.PlayOneShot(CircleMenuMoveSound);
                }
                else
                {
                    Debug.LogErrorFormat("CircleMenuMoveSound NullPointer");
                }
                break;
            case SoundMode.CircleMenuSelect:
                if (CircleMenuMoveSound)
                {
                    audio.PlayOneShot(CircleMenuSelectSound);
                }
                else
                {
                    Debug.LogErrorFormat("CircleMenuSelectSound NullPointer");
                }
                break;
            case SoundMode.CarSelectMove:
                if (CarSelectMoveSound)
                {
                    audio.PlayOneShot(CarSelectMoveSound);
                }
                else
                {
                    Debug.LogErrorFormat("CarSelectMoveSound NullPointer");
                }
                break;
            case SoundMode.CarSelect:
                if (CarSelectSound)
                {
                    audio.PlayOneShot(CarSelectSound);
                }
                else
                {
                    Debug.LogErrorFormat("CarSelectSound NullPointer");
                }
                break;
            case SoundMode.MapSelectMove:
                if (MapSelectMoveSound)
                {
                    audio.PlayOneShot(MapSelectMoveSound);
                }
                else
                {
                    Debug.LogErrorFormat("MapSelectMoveSound NullPointer");
                }
                break;
            case SoundMode.MapSelect:
                if (MapSelectSound)
                {
                    audio.PlayOneShot(MapSelectSound);
                }
                else
                {
                    Debug.LogErrorFormat("MapSelectSound NullPointer");
                }
                break;
        }
    }

    public void ChaingeBackGroundMusic(UI_BackGoundMusic type)
    {
        if (type == Type)
            return;
        else
        {
            Type = type;
        }

        BackGoundMusic.volume = BackGroundMusicVolume;
        BackGoundMusic.Stop();
        if(IsTitleOnshot)
        {
            IsTitleOnshot = false;
            StopCoroutine("TitleBackground_Cor");
        }

        switch (type)
        {
            case UI_BackGoundMusic.Title:

                StartCoroutine("TitleBackground_Cor");
                //BackGoundMusic.clip = TitleBackGroundMusic;
                //BackGoundMusic.Play();
                break;
            case UI_BackGoundMusic.MapSelect:
                BackGoundMusic.clip = MapSelectBackGroundMusic;
                BackGoundMusic.Play();
                break;
            case UI_BackGoundMusic.CarSelect:
                BackGoundMusic.clip = CarSelectBackGroundMusic;
                BackGoundMusic.Play();
                break;
        }
    }

    public enum SoundMode
    {
        Esc,
        MesaageBoxMove,
        MessageBoxSelect,
        CircleMenuMove,
        CircleMenuSelect,
        MapSelectMove,
        MapSelect,
        CarSelectMove,
        CarSelect
    }

    public enum UI_BackGoundMusic
    {
        None,
        Title,
        MapSelect,
        CarSelect
    }

    IEnumerator TitleBackground_Cor()
    {
        IsTitleOnshot = true;
        
        BackGoundMusic.PlayOneShot(OneshotTitleBackGroundMusic);

        while(true)
        {
            if(!BackGoundMusic.isPlaying)
            {
                BackGoundMusic.clip = TitleBackGroundMusic;
                BackGoundMusic.Play();
                break;
            }

            yield return null;
        }

        IsTitleOnshot = false;
        yield break;
    }

}
