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
		cout << "�û������ # ��ť/� " << actionIndex << endl;
		exit(16 + actionIndex);
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
#define COMMAND_ACTION     L"--action"
#define COMMAND_AUMI       L"--aumi"
#define COMMAND_APPNAME    L"--name"
#define COMMAND_APPID      L"--id"
#define COMMAND_EXPIREMS   L"--time"
#define COMMAND_TEXT01       L"--text01"
#define COMMAND_TEXT02       L"--text02"
#define COMMAND_TEXT03       L"--text03"
#define COMMAND_HELP       L"--help"
#define COMMAND_IMAGE      L"--image-path"
#define COMMAND_SHORTCUT   L"--only-create-shortcut"
#define COMMAND_AUDIO_STATE L"--audio-state"
#define COMMAND_ATTRIBUTE  L"--attribute"
void print_help() {
	cout << endl;
	cout << "Windows ��������֪ͨ����ƽ̨" << endl;
	cout << endl;
	cout << "�÷�:" << endl;
	cout << "\t" << "Windows_Notification_Push_Open_Platform.exe" << " [ѡ��]" << endl;
	cout << endl;
	cout << "ѡ��:" << endl;
	cout << "\t" << "--action" <<" : �ڰ�ť�����ò���" << endl;
	cout << "\t" << "*--aumi" << " : ����Ӧ���û�ģ�� ID" << endl;
	cout << "\t" << "*--name" << " : ������ʾӦ������" << endl;
	cout << "\t" << "*--id" << " : ����Ӧ��ID" << endl;
	cout << "\t" << "--time" << " : ���ù���ʱ��" << endl;
	cout << "\t" << "--text01" << " : ����֪ͨ�ĵ�һ���ı�" << endl;
	cout << "\t" << "--text02" << " : ����֪ͨ�ĵڶ����ı�" << endl;
	cout << "\t" << "--text03" << " : ����֪ͨ�ĵ������ı�" << endl;
	cout << "\t" << "--help" << " : ��ӡ����˵��" << endl;
	cout << "\t" << "--image-path" << " : ����ͼ��·��" << endl;
	cout << "\t" << "--only-create-shortcut" << " : ������Ӧ�õĿ�ݷ�ʽ" << endl;
	cout << "\t" << "--audio-state" << " : ����֪ͨ��Ƶ�Ĳ���ģʽ������ = 0���� = 1�����ѭ�� = 2" << endl;
	cout << "\t" << "--attribute" << " : ����֪ͨ��ע��" << endl;
	cout << endl;
	cout << "����*����ǵ�ѡ��Ϊ��Ҫ����ȱʧ����������޷���ȷ���е���֪ͨ�����쳣��" << endl;
}
int wmain(int argc, LPWSTR* argv) {
	if (argc == 1) {
		print_help();
		return 0;
	}
	wstring appName = L"";
	wstring appUserModelID = L"";
	wstring text01 = L"";
	wstring text02 = L"";
	wstring text03 = L"";
	wstring imagePath = L"";
	wstring attribute = L"";
	vector<wstring> actions;
	INT64 expiration = 0;
	bool onlyCreateShortcut = false;
	WinToastTemplate::AudioOption audioOption = WinToastTemplate::AudioOption::Default;
	int i;
	for (i = 1; i < argc; i++) {
		if (!wcscmp(COMMAND_IMAGE, argv[i])) {
			imagePath = argv[++i];
		}
		else if (!wcscmp(COMMAND_ACTION, argv[i])) {
			actions.push_back(argv[++i]);
		}
		else if (!wcscmp(COMMAND_EXPIREMS, argv[i])) {
			expiration = wcstol(argv[++i], NULL, 10);
		}
		else if (!wcscmp(COMMAND_APPNAME, argv[i])) {
			appName = argv[++i];
		}
		else if (!wcscmp(COMMAND_AUMI, argv[i]) || !wcscmp(COMMAND_APPID, argv[i])) {
			appUserModelID = argv[++i];
		}
		else if (!wcscmp(COMMAND_TEXT01, argv[i])) {
			text01 = argv[++i];
		}
		else if (!wcscmp(COMMAND_TEXT02, argv[i])) {
			text02 = argv[++i];
		}
		else if (!wcscmp(COMMAND_TEXT03, argv[i])) {
			text03 = argv[++i];
		}
		else if (!wcscmp(COMMAND_ATTRIBUTE, argv[i])) {
			attribute = argv[++i];
		}
		else if (!wcscmp(COMMAND_SHORTCUT, argv[i])) {
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
			wcerr << "δ֪��ѡ��: " << argv[i] << endl;
			return Results::UnhandledOption;
		}
	}
	WinToast::instance()->setAppName(appName);
	WinToast::instance()->setAppUserModelId(appUserModelID);
	if (onlyCreateShortcut) {
		if (!imagePath.empty() || !text01.empty() || !text02.empty() || !text03.empty() || actions.size() > 0 || expiration) {
			cerr << "--only-create-shortcut ѡ�����ͼƬ/�ı�/������������ڵĲ���" << endl;
			return 9;
		}
		enum WinToast::ShortcutResult result = WinToast::instance()->createShortcut();
		return result ? 16 + result : 0;
	}
	if (!WinToast::instance()->initialize()) {
		cerr << "��������ϵͳ�����ݣ���WinToast::instance()->initialize() �쳣��" << endl;
		return Results::InitializationFailure;
	}
	WinToastTemplate templ(!imagePath.empty() ? WinToastTemplate::ImageAndText02 : WinToastTemplate::Text02);
	templ.setTextField(text01, WinToastTemplate::FirstLine);
	templ.setTextField(text02, WinToastTemplate::SecondLine);
	templ.setTextField(text03, WinToastTemplate::ThirdLine);
	templ.setAudioOption(audioOption);
	templ.setAttributionText(attribute);
	templ.setImagePath(imagePath);
	for (auto const& action : actions) {
		templ.addAction(action);
	}
	if (expiration) {
		templ.setExpiration(expiration);
	}
	if (WinToast::instance()->showToast(templ, new CustomHandler()) < 0) {
		cerr << "δ������֪ͨ��";
		return Results::ToastFailed;
	}
	Sleep(expiration ? (DWORD)expiration + 1000 : 15000);
	exit(2);
}