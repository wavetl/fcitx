/***************************************************************************
 *   Copyright (C) 2010~2010 by CSSlayer                                   *
 *   wengxt@gmail.com                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/**
 * @file   ime.h
 * @author Yuking yuking_net@sohu.com
 * @date   2008-1-16
 *
 * @brief  Public Header for Input Method Develop
 *
 */
#ifndef _FCITX_IME_H_
#define _FCITX_IME_H_

#include <time.h>
#include <fcitx-utils/utf8.h>
#include <fcitx-config/hotkey.h>
#include <fcitx/ui.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_CODE_LEN    63

#define MAX_IM_NAME    (8 * UTF8_MAX_LENGTH)

#define MAX_CAND_LEN    127
#define MAX_TIPS_LEN    9

#define MAX_CAND_WORD    10
#define MAX_USER_INPUT    300

#define HOT_KEY_COUNT   2

    struct _FcitxInputContext;
    struct _FcitxInstance;
    struct _FcitxAddon;

    typedef enum _KEY_RELEASED {
        KR_OTHER = 0,
        KR_CTRL,
        KR_2ND_SELECTKEY,
        KR_3RD_SELECTKEY,
        KR_CTRL_SHIFT
    } KEY_RELEASED;

    typedef enum _INPUT_RETURN_VALUE {
        IRV_TO_PROCESS = 0, /* do something */
        IRV_FLAG_BLOCK_FOLLOWING_PROCESS = 1 << 0, /* nothing to do, actually non-zero is blocking, but you need a flag for do nothing */
        IRV_FLAG_FORWARD_KEY = 1 << 1, /* the key will be forwarded */
        IRV_FLAG_RESET_INPUT = 1 << 2, /* reset input */
        IRV_FLAG_PENDING_COMMIT_STRING = 1 << 3, /* there is something in input strStringGet buffer, commit it */
        IRV_FLAG_UPDATE_INPUT_WINDOW = 1 << 4, /* something updated in input window, let the UI update */
        IRV_FLAG_UPDATE_CANDIDATE_WORDS = 1 << 5, /* update the candidate words */
        IRV_FLAG_ENG = 1 << 6, /* special */
        IRV_FLAG_PUNC = 1 << 7, /* special */
        IRV_FLAG_DISPLAY_LAST = 1 << 8, /* special */
        IRV_FLAG_DO_PHRASE_TIPS = 1 << 9, /* special */
        /* compatible */
        IRV_DONOT_PROCESS = IRV_FLAG_FORWARD_KEY,
        IRV_COMMIT_STRING = IRV_FLAG_PENDING_COMMIT_STRING | IRV_FLAG_DO_PHRASE_TIPS,
        IRV_DO_NOTHING = IRV_FLAG_BLOCK_FOLLOWING_PROCESS,
        IRV_CLEAN = IRV_FLAG_RESET_INPUT,
        IRV_COMMIT_STRING_REMIND = IRV_FLAG_PENDING_COMMIT_STRING | IRV_FLAG_UPDATE_INPUT_WINDOW,
        IRV_DISPLAY_CANDWORDS = IRV_FLAG_UPDATE_INPUT_WINDOW | IRV_FLAG_UPDATE_CANDIDATE_WORDS,
        IRV_DONOT_PROCESS_CLEAN = IRV_FLAG_FORWARD_KEY | IRV_FLAG_RESET_INPUT,
        IRV_COMMIT_STRING_NEXT =  IRV_FLAG_PENDING_COMMIT_STRING | IRV_FLAG_UPDATE_INPUT_WINDOW,
        IRV_DISPLAY_MESSAGE = IRV_FLAG_UPDATE_INPUT_WINDOW,
        IRV_ENG = IRV_FLAG_PENDING_COMMIT_STRING | IRV_FLAG_ENG | IRV_FLAG_RESET_INPUT,
        IRV_PUNC = IRV_FLAG_PENDING_COMMIT_STRING | IRV_FLAG_PUNC | IRV_FLAG_RESET_INPUT,
        IRV_DISPLAY_LAST = IRV_FLAG_UPDATE_INPUT_WINDOW | IRV_FLAG_DISPLAY_LAST
    } INPUT_RETURN_VALUE;

    /**
     * @brief Fcitx Input Method class, it can register more than one input
     *        method in create function
     **/
    typedef struct _FcitxIMClass {
        void*              (*Create) (struct _FcitxInstance* instance);
        void               (*Destroy) (void *arg);
    } FcitxIMClass;

    typedef boolean            (*FcitxIMInit) (void *arg);
    typedef void               (*FcitxIMResetIM) (void *arg);
    typedef INPUT_RETURN_VALUE (*FcitxIMDoInput) (void *arg, FcitxKeySym, unsigned int);
    typedef INPUT_RETURN_VALUE (*FcitxIMGetCandWords) (void *arg);
    typedef boolean            (*FcitxIMPhraseTips) (void *arg);
    typedef void               (*FcitxIMSave) (void *arg);
    typedef void               (*FcitxIMReloadConfig) (void *arg);

    /**
     * @brief Fcitx Input method instance
     **/
    typedef struct _FcitxIM {
        /**
         * @brief The name that can be display on the UI
         **/
        char               strName[MAX_IM_NAME + 1];
        /**
         * @brief icon name used to find icon
         **/
        char               strIconName[MAX_IM_NAME + 1];
        /**
         * @brief reset im status
         **/
        FcitxIMResetIM ResetIM;
        /**
         * @brief process key input
         **/
        FcitxIMDoInput DoInput;
        /**
         * @brief update candidate works function
         **/
        FcitxIMGetCandWords GetCandWords;
        /**
         * @brief phrase tips function
         **/
        FcitxIMPhraseTips PhraseTips;
        /**
         * @brief save function
         **/
        FcitxIMSave Save;
        /**
         * @brief init function
         **/
        FcitxIMInit Init;
        /**
         * @brief reload config function
         **/
        FcitxIMReloadConfig ReloadConfig;
        /**
         * @brief private data can be set by UI implementation
         **/
        void* uiprivate;
        /**
         * @brief the pointer to im class
         **/
        void* klass;
        /**
         * @brief the priority order
         **/
        int iPriority;
        /**
         * @brief private data for this input method
         **/
        void* priv;
    } FcitxIM;

    typedef enum _FcitxKeyEventType {
        FCITX_PRESS_KEY,
        FCITX_RELEASE_KEY
    } FcitxKeyEventType;

    /**
     * @brief Global Input State, including displayed message.
     **/
    typedef struct _FcitxInputState {
        long unsigned int lastKeyPressedTime;
        boolean bIsDoInputOnly;
        KEY_RELEASED keyReleased;
        int iCodeInputCount;
        char strCodeInput[MAX_USER_INPUT + 1];
        char strStringGet[MAX_USER_INPUT + 1];  //保存输入法返回的需要送到客户程序中的字串
        boolean bIsInRemind;

        time_t timeStart;
        int iCursorPos;
        boolean bShowCursor;
        int iHZInputed;
        int lastIsSingleHZ;
        boolean bLastIsNumber;
        boolean bStartRecordType;

        /* the ui message part, if there is something in it, then it will be shown */
        struct _CandidateWordList* candList;
        Messages* msgPreedit;
        Messages* msgAuxUp;
        Messages* msgAuxDown;
    } FcitxInputState;

    boolean IsHotKey(FcitxKeySym sym, int state, HOTKEYS * hotkey);
    char* GetOutputString(FcitxInputState* input);
    struct _FcitxIM* GetCurrentIM(struct _FcitxInstance *instance);
    void EnableIM(struct _FcitxInstance* instance, struct _FcitxInputContext* ic, boolean keepState);
    void ResetInput (struct _FcitxInstance* instance);
    void CleanInputWindow(struct _FcitxInstance *instance);
    void CleanInputWindowUp(struct _FcitxInstance *instance);
    void CleanInputWindowDown(struct _FcitxInstance *instance);
    /**
     * @brief Sometimes, we use INPUT_RETURN_VALUE not from ProcessKey, so use this function to do the correct thing.
     *
     * @param instance fcitx instance
     * @param retVal input return val
     * @return void
     **/
    void ProcessInputReturnValue(
        struct _FcitxInstance* instance,
        INPUT_RETURN_VALUE retVal
    );
    void FcitxRegisterIM(struct _FcitxInstance *instance,
                         void *addonInstance,
                         const char* name,
                         const char* iconName,
                         FcitxIMInit Init,
                         FcitxIMResetIM ResetIM,
                         FcitxIMDoInput DoInput,
                         FcitxIMGetCandWords GetCandWords,
                         FcitxIMPhraseTips PhraseTips,
                         FcitxIMSave Save,
                         FcitxIMReloadConfig ReloadConfig,
                         void *priv,
                         int priority
                        );

    INPUT_RETURN_VALUE ProcessKey(struct _FcitxInstance* instance, FcitxKeyEventType event, long unsigned int timestamp, FcitxKeySym sym, unsigned int state);
    void ForwardKey(struct _FcitxInstance* instance, struct _FcitxInputContext* ic, FcitxKeyEventType event, FcitxKeySym sym, unsigned int state);
    void SaveAllIM (struct _FcitxInstance* instance);
    void ReloadConfig(struct _FcitxInstance* instance);
    void SwitchIM (struct _FcitxInstance* instance, int index);
    int CheckChooseKey (FcitxKeySym sym, int state, const char* strChoose);

#ifdef __cplusplus
}
#endif

#endif

// kate: indent-mode cstyle; space-indent on; indent-width 0;
