using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.IO.Ports;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Windows.Input;

namespace Tester_ver1_WinProg
{
    public class ViewModel : INotifyPropertyChanged
    {
        /*модель*/
        HandlerCOM handlerCOM;

        /*кнопки*/
        public ICommand RefreshBtnClick { get; set; }
        private bool _refreshBtnIsEnable;
        public bool RefreshBtnIsEnable
        {
            get => _refreshBtnIsEnable;
            set
            {
                if (_refreshBtnIsEnable == value) return;
                _refreshBtnIsEnable = value;
                OnPropertyChanged();
            }
        }
        public ICommand OpenBtnClick { get; set; }
        private bool _openBtnIsEnable;
        public bool OpenBtnIsEnable
        {
            get => _openBtnIsEnable;
            set
            {
                if (_openBtnIsEnable == value) return;
                _openBtnIsEnable = value;
                OnPropertyChanged();
            }
        }
        public ICommand CloseBtnClick { get; set; }
        private bool _closeBtnIsEnable;
        public bool CloseBtnIsEnable
        {
            get => _closeBtnIsEnable;
            set
            {
                if (_closeBtnIsEnable == value) return;
                _closeBtnIsEnable = value;
                OnPropertyChanged();
            }
        }
        public ICommand SendBtnClick { get; set; }
        private bool _sendBtnIsEnable;
        public bool SendBtnIsEnable
        {
            get => _sendBtnIsEnable;
            set
            {
                if (_sendBtnIsEnable == value) return;
                _sendBtnIsEnable = value;
                OnPropertyChanged();
            }
        }
        public ICommand ClearBtnClick { get; set; }
        private bool _clearBtnIsEnable;
        public bool ClearBtnIsEnable
        {
            get => _clearBtnIsEnable;
            set
            {
                if (_clearBtnIsEnable == value) return;
                _clearBtnIsEnable = value;
                OnPropertyChanged();
            }
        }

        public ICommand ButtonPlusClick { get; set; }

        public ICommand ButtonMinusClick { get; set; }

        public ICommand ButtonFleshClick { get; set; }

        public ICommand ButtonOnOffClick { get; set; }
        private string _buttonOnOffContent;
        public string ButtonOnOffContent
        {
            get => _buttonOnOffContent;
            set
            {
                if (_buttonOnOffContent == value) return;
                _buttonOnOffContent = value;
                OnPropertyChanged();
            }
        }
        /*чеки*/
        private bool _rtsCheckClick;
        public bool RtsCheckClick
        {
            get => _rtsCheckClick;
            set
            {
                if (_rtsCheckClick == value) return;
                _rtsCheckClick = value;
                OnPropertyChanged();
            }
        }

        private bool _dtrCheckClick;
        public bool DtrCheckClick
        {
            get => _dtrCheckClick;
            set
            {
                if (_dtrCheckClick == value) return;
                _dtrCheckClick = value;
                OnPropertyChanged();
            }
        }

        private bool _checkFlash;
        public bool CheckFlash
        {
            get => _checkFlash;
            set
            {
                if (_checkFlash == value) return;
                _checkFlash = value;
                OnPropertyChanged();
            }
        }

        /*списки*/
        public ObservableCollection<string> PortCombo { get; set; } = new ObservableCollection<string>();
        private string _portComboSelectItem;
        public string PortComboSelectItem
        {
            get => _portComboSelectItem;
            set
            {
                if (_portComboSelectItem == value) return;
                _portComboSelectItem = value;
                OnPropertyChanged();
            }
        }
        private int _portComboSelectIndex;
        public int PortComboSelectIndex
        {
            get => _portComboSelectIndex;
            set
            {
                if (_portComboSelectIndex == value) return;
                _portComboSelectIndex = value;
                OnPropertyChanged();
            }
        }
        public ObservableCollection<int> BaudCombo { get; set; } = new ObservableCollection<int>();
        private int _baudComboSelectItem;
        public int BaudComboSelectItem
        {
            get => _baudComboSelectItem;
            set
            {
                if (_baudComboSelectItem == value) return;
                _baudComboSelectItem = value;
                OnPropertyChanged();
            }
        }
        public ObservableCollection<Parity> ParityCombo { get; set; } = new ObservableCollection<Parity>();
        private Parity _parityComboSelectItem;
        public Parity ParityComboSelectItem
        {
            get => _parityComboSelectItem;
            set
            {
                if (_parityComboSelectItem == value) return;
                _parityComboSelectItem = value;
                OnPropertyChanged();
            }
        }
        public ObservableCollection<int> DataBitsCombo { get; set; } = new ObservableCollection<int>();
        private int _dataBitsComboSelectItem;
        public int DataBitsComboSelectItem
        {
            get => _dataBitsComboSelectItem;
            set
            {
                if (_dataBitsComboSelectItem == value) return;
                _dataBitsComboSelectItem = value;
                OnPropertyChanged();
            }
        }
        public ObservableCollection<StopBits> StopBitsCombo { get; set; } = new ObservableCollection<StopBits>();
        private StopBits _stopBitsComboSelectItem;
        public StopBits StopBitsComboSelectItem
        {
            get => _stopBitsComboSelectItem;
            set
            {
                if (_stopBitsComboSelectItem == value) return;
                _stopBitsComboSelectItem = value;
                OnPropertyChanged();
            }
        }
        public ObservableCollection<Handshake> HandshakeCombo { get; set; } = new ObservableCollection<Handshake>();
        private Handshake _handshakeComboSelectItem;
        public Handshake HandshakeComboSelectItem
        {
            get => _handshakeComboSelectItem;
            set
            {
                if (_handshakeComboSelectItem == value) return;
                _handshakeComboSelectItem = value;
                OnPropertyChanged();
            }
        }

        /*тексты*/
        private string _logText;
        public string LogText
        {
            get => _logText;
            set
            {
                if (_logText == value) return;
                _logText = value;
                OnPropertyChanged();
            }
        }
        private string _sendBox;
        public string SendBox
        {
            get => _sendBox;
            set
            {
                if (_sendBox == value) return;
                _sendBox = value;
                OnPropertyChanged();
            }
        }

        private string _logInfa;
        public string LogInfa
        {
            get => _logInfa;
            set
            {
                if (_logInfa == value) return;
                _logInfa = value;
                OnPropertyChanged();
            }
        }

        
        //private string _pwrStep;
        //public string PWRStep
        //{
        //    get => _pwrStep;
        //    set
        //    {
        //        if (_pwrStep == value) return;
        //        _pwrStep = value;
        //        OnPropertyChanged();
        //    }
        //}

        //private string _pwrFill;
        //public string PWRFill
        //{
        //    get => _pwrFill;
        //    set
        //    {
        //        if (_pwrFill == value) return;
        //        _pwrFill = value;
        //        OnPropertyChanged();
        //    }
        //}

        //private string _currTemp;
        //public string CurrTemp
        //{
        //    get => _currTemp;
        //    set
        //    {
        //        if (_currTemp == value) return;
        //        _currTemp = value;
        //        OnPropertyChanged();
        //    }
        //}

        /*вход*/
        public ViewModel()
        {
            InitButtons();
            // Для теста добавим элемент по умолчанию
            //Items.Add("Первый элемент");
            //LogText = "[eqyz";
            //PortCombo.Add(LogText);
            handlerCOM = new HandlerCOM(this);

            MainWindow.OnIsClosed += Closed;
        }
        private void InitButtons()
        {
            RefreshBtnClick = new RelayCommand(OnRefreshBtnClick, CanRefreshBtnClick);
            OpenBtnClick = new RelayCommand(OnOpenBtnClick, CanOpenBtnClick);
            CloseBtnClick = new RelayCommand(OnCloseBtnClick, CanCloseBtnClick);
            SendBtnClick = new RelayCommand(OnSendBtnClick, CanSendBtnClick);
            ClearBtnClick = new RelayCommand(OnClearBtnClick, CanClearBtnClick);
            //ButtonPlusClick = new RelayCommand(OnButtonPlusClick, CanButtonPlusClick);
            //ButtonMinusClick = new RelayCommand(OnButtonMinusClick, CanButtonMinusClick);
            //ButtonFleshClick = new RelayCommand(OnButtonFleshClick, CanButtonFleshClick);
            ButtonOnOffClick = new RelayCommand(OnButtonOnOffClick, CanButtonOnOffClick);
        }
        /*обработка кнопок*/
        public void OnRefreshBtnClick(object parameter)
        {
            handlerCOM.RefreshBtnClick();
        }
        private bool CanRefreshBtnClick(object parameter)
        {
            return true;
        }

        public void OnOpenBtnClick(object parameter)
        {
            handlerCOM.OpenBtnClick();
        }
        private bool CanOpenBtnClick(object parameter)
        {
            return true;
        }

        public void OnCloseBtnClick(object parameter)
        {
            handlerCOM.CloseBtnClick();
        }
        private bool CanCloseBtnClick(object parameter)
        {
            return true;
        }

        public void OnSendBtnClick(object parameter)
        {
            handlerCOM.SendBtnClick();
        }
        private bool CanSendBtnClick(object parameter)
        {
            return true;
        }

        public void OnClearBtnClick(object parameter)
        {
            handlerCOM.ClearBtnClick();
        }
        private bool CanClearBtnClick(object parameter)
        {
            return true;
        }

        public void OnButtonOnOffClick(object parameter)
        {
            handlerCOM.BtnComandClick(handlerCOM.btnComandOnOff);
        }
        private bool CanButtonOnOffClick(object parameter)
        {
            return true;
        }
        /*обработчик*/
        private void Closed(bool flag)
        {
            handlerCOM.Closed(flag);
        }

        //public void OnButtonPlusClick(object parameter)
        //{
        //    handlerCOM.BtnComandClick(handlerCOM.btnComandPlus);
        //}
        //private bool CanButtonPlusClick(object parameter)
        //{
        //    return true;
        //}

        //public void OnButtonMinusClick(object parameter)
        //{
        //    handlerCOM.BtnComandClick(handlerCOM.btnComandMinus);
        //}
        //private bool CanButtonMinusClick(object parameter)
        //{
        //    return true;
        //}

        //public void OnButtonFleshClick(object parameter)
        //{
        //    handlerCOM.BtnComandClick(handlerCOM.btnComandFlesh);
        //}
        //private bool CanButtonFleshClick(object parameter)
        //{
        //    return true;
        //}

        /*обработка текстов - евенты*/
        public event PropertyChangedEventHandler PropertyChanged;
        protected void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}
