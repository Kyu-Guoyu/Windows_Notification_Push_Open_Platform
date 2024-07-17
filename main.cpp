#include <windows.h>
#include "wintoastlib.h"
#include <string>
using namespace WinToastLib;
using namespace std;
class CustomHandler : public IWinToastHandler {
public:
	void toastActivated() const {
		cout << "用户点击了通知" << endl;
		exit(0);
	}
	void toastActivated(int actionIndex) const {
		cout << "用户点击了第" << (actionIndex+1) << "个按钮"  << endl;
		exit((actionIndex + 1) + 21);
	}
	void toastDismissed(WinToastDismissalReason state) const {
		switch (state) {
		case UserCanceled:
			cout << "用户忽略了通知" << endl;
			exit(1);
			break;
		case TimedOut:
			cout << "通知超时" << endl;
			exit(2);
			break;
		case ApplicationHidden:
			cout << "应用程序使用了 ToastNotifier.hide() 来隐藏了通知" << endl;
			exit(3);
			break;
		default:
			cout << "通知并未激活" << endl;
			exit(4);
			break;
		}
	}
	void toastFailed() const {
		cout << "未能显示当前通知" << endl;
		exit(5);
	}
};
enum Results
{
	ToastClicked,             //用户点击了通知
	ToastDismissed,           //用户忽略了通知
	ToastTimeOut,             //通知超时
	ToastHided,               //应用程序隐藏了通知
	ToastNotActivated,        //通知未激活
	ToastFailed,              //通知失败
	SystemNotSupported,       //系统不支持通知
	UnhandledOption,          //未处理选项
	MultipleTextNotSupported, //提供了多个文本
	InitializationFailure,    //通知管理器初始化失败
	ToastNotLaunched          //通知未能启推
};
#define COMMAND_APPUSERMODELID       L"--aumi"
#define COMMAND_APPDISPLAYNAME    L"--name"
#define COMMAND_APPID      L"--id"
#define COMMAND_TIME   L"--time"
#define COMMAND_TEXT01       L"--text01"
#define COMMAND_TEXT02       L"--text02"
#define COMMAND_BUTTON     L"--button"
#define COMMAND_IMAGE_PATH      L"--image-path"
#define COMMAND_AUDIO_STATE L"--audio-state"
#define COMMAND_ATTRIBUTE  L"--attribute"
#define COMMAND_ONLY_CREATE_SHORTCUT   L"--ocs"
#define COMMAND_HELP       L"--help"
void print_help() {
	wcout << endl;
	wcout << endl;
	wcout << "Windows 操作中心通知开放平台" << endl;
	wcout << endl;
	wcout << "用法:" << endl;
	wcout << "\t" << "Windows_Notification_Push_Open_Platform.exe" << " 【选项】" << endl;
	wcout << endl;
	wcout << "选项:" << endl;
	wcout << "\t" << COMMAND_APPUSERMODELID << " ：设置应用用户模型 ID*" << endl;
	wcout << "\t" << COMMAND_APPDISPLAYNAME << " ：设置显示应用名称*" << endl;
	wcout << "\t" << COMMAND_APPID << " ：设置应用ID*" << endl;
	wcout << "\t" << COMMAND_TIME << " ：设置显示时间（仅在0≤时间≤7000时有效）" << endl;
	wcout << "\t" << COMMAND_TEXT01 << " ：设置通知的第一行文本" << endl;
	wcout << "\t" << COMMAND_TEXT02 << " ：设置通知的第二行文本" << endl;
	wcout << "\t" << COMMAND_BUTTON << " ：设置通知中的按钮" << endl;
	wcout << "\t" << COMMAND_IMAGE_PATH << " ：设置图像路径" << endl;
	wcout << "\t" << COMMAND_AUDIO_STATE << " ：设置通知音频的播放模式：单次 = 0，无 = 1，多次循环 = 2" << endl;
	wcout << "\t" << COMMAND_ATTRIBUTE << " ：设置通知的注释（也可当作第三行使用[Doge]）" << endl;
	wcout << "\t" << COMMAND_ONLY_CREATE_SHORTCUT << " ：仅创建应用的快捷方式" << endl;
	wcout << "\t" << COMMAND_HELP << " ：打印帮助说明" << endl;
	wcout << endl;
	wcout << "注意事项：" << endl;
	wcout << "\t" << "被“*”标记的选项为必要选项，如果缺失，程序可能无法正确运行导致通知启推异常。" << endl;
	wcout << endl;
}
int wmain(int argc, LPWSTR* argv) {
	if (argc == 1) {
		cout << "缺少必要选项" << endl;
		return 0;
	}
	wstring appName = L"";
	wstring appUserModelID = L"";
	wstring text01 = L"";
	wstring text02 = L"";
	wstring imagePath = L"";
	wstring attribute = L"";
	vector<wstring> buttons;
	INT64 expiration = 0;
	bool onlyCreateShortcut = false;
	WinToastTemplate::AudioOption audioOption = WinToastTemplate::AudioOption::Default;
	int i;
	for (i = 1; i < argc; i++) {
		if (!wcscmp(COMMAND_IMAGE_PATH, argv[i])) {
			imagePath = argv[++i];
		}
		else if (!wcscmp(COMMAND_BUTTON, argv[i])) {
			buttons.push_back(argv[++i]);
		}
		else if (!wcscmp(COMMAND_TIME, argv[i])) {
			expiration = wcstol(argv[++i], NULL, 10);
		}
		else if (!wcscmp(COMMAND_APPDISPLAYNAME, argv[i])) {
			appName = argv[++i];
		}
		else if (!wcscmp(COMMAND_APPUSERMODELID, argv[i]) || !wcscmp(COMMAND_APPID, argv[i])) {
			appUserModelID = argv[++i];
		}
		else if (!wcscmp(COMMAND_TEXT01, argv[i])) {
			text01 = argv[++i];
		}
		else if (!wcscmp(COMMAND_TEXT02, argv[i])) {
			text02 = argv[++i];
		}
		else if (!wcscmp(COMMAND_ATTRIBUTE, argv[i])) {
			attribute = argv[++i];
		}
		else if (!wcscmp(COMMAND_ONLY_CREATE_SHORTCUT, argv[i])) {
			onlyCreateShortcut = true;
		}
		else if (!wcscmp(COMMAND_AUDIO_STATE, argv[i])) {
			audioOption = static_cast<WinToastTemplate::AudioOption>(stoi(argv[++i]));
		}
		else if (!wcscmp(COMMAND_HELP, argv[i])) {
			print_help();
			return 0;
		}
		else {
			wcerr << "未知或错误的选项" << endl;
			wcerr << "语法错误 " << endl;
			return Results::UnhandledOption;
		}
	}
	WinToast::instance()->setAppName(appName);
	WinToast::instance()->setAppUserModelId(appUserModelID);
	if (onlyCreateShortcut) {
		if (!imagePath.empty() || !text01.empty() || !text02.empty() || buttons.size() > 0 || expiration) {
			cerr << "--ocs 选项不接受图片/文本/动作参数或过期的参数" << endl;
			return 9;
		}
		enum WinToast::ShortcutResult result = WinToast::instance()->createShortcut();
		return result ? 16 + result : 0;
	}
	if (!WinToast::instance()->initialize()) {
		cerr << "缺少必要选项或您的系统不兼容或初始化错误！" << endl;
		return Results::InitializationFailure;
	}
	WinToastTemplate templ(!imagePath.empty() ? WinToastTemplate::ImageAndText02 : WinToastTemplate::Text02);
	templ.setTextField(text01, WinToastTemplate::FirstLine);
	templ.setTextField(text02, WinToastTemplate::SecondLine);
	templ.setAudioOption(audioOption);
	templ.setAttributionText(attribute);
	templ.setImagePath(imagePath);
	for (auto const& buttons : buttons) {
		templ.addAction(buttons);
	}
	if (expiration) {
		templ.setExpiration(expiration);
	}
	if (WinToast::instance()->showToast(templ, new CustomHandler()) < 0) {
		cerr << "未能启推通知！";
		return Results::ToastFailed;
	}
	Sleep(expiration ? (DWORD)expiration + 1000 : 15000);
	exit(21);
}