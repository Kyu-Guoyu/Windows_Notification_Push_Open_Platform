#include <windows.h>
#include "wintoastlib.h"
#include <string>
using namespace WinToastLib;
using namespace std;
class CustomHandler : public IWinToastHandler {
public:
	void toastActivated() const {
		cout << "�û������֪ͨ" << endl;
		exit(0);
	}
	void toastActivated(int actionIndex) const {
		cout << "�û�����˵�" << (actionIndex+1) << "����ť"  << endl;
		exit((actionIndex + 1) + 21);
	}
	void toastDismissed(WinToastDismissalReason state) const {
		switch (state) {
		case UserCanceled:
			cout << "�û�������֪ͨ" << endl;
			exit(1);
			break;
		case TimedOut:
			cout << "֪ͨ��ʱ" << endl;
			exit(2);
			break;
		case ApplicationHidden:
			cout << "Ӧ�ó���ʹ���� ToastNotifier.hide() ��������֪ͨ" << endl;
			exit(3);
			break;
		default:
			cout << "֪ͨ��δ����" << endl;
			exit(4);
			break;
		}
	}
	void toastFailed() const {
		cout << "δ����ʾ��ǰ֪ͨ" << endl;
		exit(5);
	}
};
enum Results
{
	ToastClicked,             //�û������֪ͨ
	ToastDismissed,           //�û�������֪ͨ
	ToastTimeOut,             //֪ͨ��ʱ
	ToastHided,               //Ӧ�ó���������֪ͨ
	ToastNotActivated,        //֪ͨδ����
	ToastFailed,              //֪ͨʧ��
	SystemNotSupported,       //ϵͳ��֧��֪ͨ
	UnhandledOption,          //δ����ѡ��
	MultipleTextNotSupported, //�ṩ�˶���ı�
	InitializationFailure,    //֪ͨ��������ʼ��ʧ��
	ToastNotLaunched          //֪ͨδ������
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
	wcout << "Windows ��������֪ͨ����ƽ̨" << endl;
	wcout << endl;
	wcout << "�÷�:" << endl;
	wcout << "\t" << "Windows_Notification_Push_Open_Platform.exe" << " ��ѡ�" << endl;
	wcout << endl;
	wcout << "ѡ��:" << endl;
	wcout << "\t" << COMMAND_APPUSERMODELID << " ������Ӧ���û�ģ�� ID*" << endl;
	wcout << "\t" << COMMAND_APPDISPLAYNAME << " ��������ʾӦ������*" << endl;
	wcout << "\t" << COMMAND_APPID << " ������Ӧ��ID*" << endl;
	wcout << "\t" << COMMAND_TIME << " ��������ʾʱ�䣨����0��ʱ���7000ʱ��Ч��" << endl;
	wcout << "\t" << COMMAND_TEXT01 << " ������֪ͨ�ĵ�һ���ı�" << endl;
	wcout << "\t" << COMMAND_TEXT02 << " ������֪ͨ�ĵڶ����ı�" << endl;
	wcout << "\t" << COMMAND_BUTTON << " ������֪ͨ�еİ�ť" << endl;
	wcout << "\t" << COMMAND_IMAGE_PATH << " ������ͼ��·��" << endl;
	wcout << "\t" << COMMAND_AUDIO_STATE << " ������֪ͨ��Ƶ�Ĳ���ģʽ������ = 0���� = 1�����ѭ�� = 2" << endl;
	wcout << "\t" << COMMAND_ATTRIBUTE << " ������֪ͨ��ע�ͣ�Ҳ�ɵ���������ʹ��[Doge]��" << endl;
	wcout << "\t" << COMMAND_ONLY_CREATE_SHORTCUT << " ��������Ӧ�õĿ�ݷ�ʽ" << endl;
	wcout << "\t" << COMMAND_HELP << " ����ӡ����˵��" << endl;
	wcout << endl;
	wcout << "ע�����" << endl;
	wcout << "\t" << "����*����ǵ�ѡ��Ϊ��Ҫѡ����ȱʧ����������޷���ȷ���е���֪ͨ�����쳣��" << endl;
	wcout << endl;
}
int wmain(int argc, LPWSTR* argv) {
	if (argc == 1) {
		cout << "ȱ�ٱ�Ҫѡ��" << endl;
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
			wcerr << "δ֪������ѡ��" << endl;
			wcerr << "�﷨���� " << endl;
			return Results::UnhandledOption;
		}
	}
	WinToast::instance()->setAppName(appName);
	WinToast::instance()->setAppUserModelId(appUserModelID);
	if (onlyCreateShortcut) {
		if (!imagePath.empty() || !text01.empty() || !text02.empty() || buttons.size() > 0 || expiration) {
			cerr << "--ocs ѡ�����ͼƬ/�ı�/������������ڵĲ���" << endl;
			return 9;
		}
		enum WinToast::ShortcutResult result = WinToast::instance()->createShortcut();
		return result ? 16 + result : 0;
	}
	if (!WinToast::instance()->initialize()) {
		cerr << "ȱ�ٱ�Ҫѡ�������ϵͳ�����ݻ��ʼ������" << endl;
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
		cerr << "δ������֪ͨ��";
		return Results::ToastFailed;
	}
	Sleep(expiration ? (DWORD)expiration + 1000 : 15000);
	exit(21);
}