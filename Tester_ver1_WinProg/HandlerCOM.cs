using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace Tester_ver1_WinProg
{
    public class HandlerCOM
    {
        private ViewModel vm;
        private bool OneWriteFlag = false;
        private string OnFlag = "Выкл";
        private string OffFlag = "Вкл";

        private int lengthCode = 500; /*количество символов в коде пакета usart*/
        private char flagStop = '#';  /*символ стоп пакета usart*/
        private string comandButton = "But";
        private string comandFlesh = "Flh";
        private string comandPwr = "Pwr";
        private string comandOffOn = "Off";
        private string comandTemperature = "Tem";

        //public string btnComandPlus = "BtnPlus";
        //public string btnComandMinus = "BtnMinus";
        //public string btnComandFlesh = "BtnFlesh";
        public string btnComandOnOff = "BtnOnOff";
        public HandlerCOM(ViewModel _vm)
        {
            vm = _vm;
            InitSerialPort();
        }

        SerialPort port;
        /**/
        private ObservableCollection<string> PortCombo;
        private ObservableCollection<int> BaudCombo;
        private ObservableCollection<Parity> ParityCombo;
        private ObservableCollection<int> DataBitsCombo;
        private ObservableCollection<StopBits> StopBitsCombo;
        private ObservableCollection<Handshake> HandshakeCombo;

        public void InitSerialPort()
        {
            port = new SerialPort();
            SetPort();

            vm.CloseBtnIsEnable = false;
            vm.SendBtnIsEnable = false;

            vm.RefreshBtnIsEnable = true;
            vm.OpenBtnIsEnable = true;
            vm.ClearBtnIsEnable = true;

            vm.ButtonOnOffContent = OffFlag;
        }

        public void SetPort()
        {
            string[] ports = SerialPort.GetPortNames().OrderBy(n => n).ToArray();
            PortCombo = new ObservableCollection<string>(ports);
            vm.PortCombo = PortCombo;
            if (ports.Length > 0)
            {
                vm.PortComboSelectIndex = 0;
            }

            int[] listBaund = new int[] { 300, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200 };
            BaudCombo = new ObservableCollection<int>(listBaund);
            vm.BaudCombo = BaudCombo;
            vm.BaudComboSelectItem = 19200;

            IEnumerable<Parity> parity = Enum.GetValues(typeof(Parity)).Cast<Parity>();
            ParityCombo = new ObservableCollection<Parity>(parity);
            vm.ParityCombo = ParityCombo;
            vm.ParityComboSelectItem = Parity.None;

            int[] listBits = new int[] { 5, 6, 7, 8 };
            DataBitsCombo = new ObservableCollection<int>(listBits);
            vm.DataBitsCombo = DataBitsCombo;
            vm.DataBitsComboSelectItem = 8;

            IEnumerable<StopBits> stopBits = Enum.GetValues(typeof(StopBits)).Cast<StopBits>();
            StopBitsCombo = new ObservableCollection<StopBits>(stopBits);
            vm.StopBitsCombo = StopBitsCombo;
            vm.StopBitsComboSelectItem = StopBits.One;

            IEnumerable<Handshake> handshake = Enum.GetValues(typeof(Handshake)).Cast<Handshake>();
            HandshakeCombo = new ObservableCollection<Handshake>(handshake);
            vm.HandshakeCombo = HandshakeCombo;
            vm.HandshakeComboSelectItem = Handshake.None;
        }
        public void RefreshBtnClick()
        {
            int selint = vm.PortComboSelectIndex;
            string sel = selint.ToString();
            string[] ports = SerialPort.GetPortNames().OrderBy(n => n).ToArray();
            PortCombo = new ObservableCollection<string>(ports);
            vm.PortCombo = PortCombo;
            if (!string.IsNullOrEmpty(sel) && ports.Contains(sel))
            {
                vm.PortComboSelectItem = sel;
            }
            else if (ports.Length > 0)
            {
                vm.PortComboSelectIndex = 0;
            }
        }

        public void OpenBtnClick()
        {
            if (port != null && port.IsOpen)
            {
                return;
            }

            if (vm.PortComboSelectItem == null)
            {
                MessageBox.Show("Выберите порт", "Ошибка", MessageBoxButton.OK, MessageBoxImage.Warning);
                return;
            }

            try
            {
                string portName = vm.PortComboSelectItem.ToString();
                int baud = Convert.ToInt32(vm.BaudComboSelectItem);
                Parity parity = vm.ParityComboSelectItem;
                int dataBits = Convert.ToInt32(vm.DataBitsComboSelectItem);
                StopBits stopBits = (StopBits)vm.StopBitsComboSelectItem;
                Handshake handshake = (Handshake)vm.HandshakeComboSelectItem;

                port = new SerialPort(portName, baud, parity, dataBits, stopBits)
                {
                    Encoding = Encoding.UTF8,
                    Handshake = handshake,
                    ReadTimeout = 1500,
                    WriteTimeout = 1500,
                    DtrEnable = vm.DtrCheckClick == true,
                    RtsEnable = vm.RtsCheckClick == true,
                    NewLine = "\n"
                };

                port.DataReceived += PortDataReceived;
                port.ErrorReceived += PortErrorReceived;
                port.Open();

                vm.LogInfa = string.Format("Открыто {0} {1} {2} {3} {4}", portName, baud, parity, dataBits, stopBits);
                vm.OpenBtnIsEnable = false;
                vm.CloseBtnIsEnable = true;
                vm.SendBtnIsEnable = true;

            }
            catch (Exception ex)
            {
                MessageBox.Show("Ошибка открытия: " + ex.Message, "Ошибка", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private void PortDataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            try
            {
                if (port == null)
                {
                    return;
                }

                string data = port.ReadExisting();
                AppendReceived(data);
            }
            catch (Exception ex)
            {
                vm.LogInfa = ("Ошибка чтения: " + ex.Message);
            }
        }

        private void PortErrorReceived(object sender, SerialErrorReceivedEventArgs e)
        {
            vm.LogInfa = ("Ошибка порта: " + e.EventType);
        }

        private void AppendReceived(string data)
        {
            vm.LogText = data;
            DecodUSART(data, lengthCode);
        }

        private void DecodUSART(string data, int lengthCode)
        {
            char[] arrChar = data.ToCharArray();
            string commandCodeUSART = string.Empty;
            string arrDataUSART = string.Empty;

            for (int i = 0; i < arrChar.Length; i++)
            {
                if (i < lengthCode)
                {
                    commandCodeUSART += arrChar[i];
                }
                else
                {
                    if (arrChar[i] == flagStop)
                    {
                        break;
                    }
                    else
                    {
                        arrDataUSART += arrChar[i];
                    }
                }
            }

            if (commandCodeUSART == comandButton)
            {
                //vm.PWRStep = arrDataUSART;
                vm.CheckFlash = false;
            }

            if (commandCodeUSART == comandPwr)
            {
                //vm.PWRFill = arrDataUSART;
            }

            if (commandCodeUSART == comandFlesh)
            {
                vm.CheckFlash = true;
            }

            if (commandCodeUSART == comandOffOn)
            {
                if (arrDataUSART == "0")
                {
                    vm.ButtonOnOffContent = OffFlag;
                }
                else
                {
                    vm.ButtonOnOffContent = OnFlag;
                }

            }

            if (commandCodeUSART == comandTemperature)
            {
                //vm.CurrTemp = arrDataUSART;
            }

            vm.LogText = commandCodeUSART;
        }


        public void CloseBtnClick()
        {
            OneWriteFlag = false;
            ClosePort();
        }

        private void ClosePort()
        {
            if (port == null)
            {
                return;
            }
            try
            {
                port.DataReceived -= PortDataReceived;
                port.ErrorReceived -= PortErrorReceived;

                if (port.IsOpen)
                {
                    port.Close();
                }

                port.Dispose();
                vm.LogInfa = "Закрытый порт";
            }
            catch (Exception ex)
            {
                vm.LogInfa = "Ошибка закрытия: " + ex.Message;
            }
            finally
            {
                port = null;
                vm.OpenBtnIsEnable = true;
                vm.CloseBtnIsEnable = false;
                vm.SendBtnIsEnable = false;

            }
        }

        public void BtnComandClick(string comand)
        {
            if (port == null || !port.IsOpen)
            {
                MessageBox.Show("Порт не открыт", "Ошибка", MessageBoxButton.OK, MessageBoxImage.Warning);
                return;
            }

            comand += flagStop;

            try
            {
                if (OneWriteFlag == false)
                {
                    port.WriteLine(comand);
                    OneWriteFlag = true;
                }
                port.WriteLine(comand);
                vm.LogText = ">>> " + comand;
            }
            catch (Exception ex)
            {
                vm.LogInfa = "Ошибка записи: " + ex.Message;
            }
        }

        public void SendBtnClick()
        {
            if (port == null || !port.IsOpen)
            {
                MessageBox.Show("Порт не открыт", "Ошибка", MessageBoxButton.OK, MessageBoxImage.Warning);
                return;
            }

            string text = vm.SendBox ?? string.Empty;
            try
            {
                port.WriteLine(text);
                vm.LogText = ">>> " + text;
            }
            catch (Exception ex)
            {
                vm.LogInfa = "Ошибка записи: " + ex.Message;
            }
        }

        public void ClearBtnClick()
        {
            vm.LogText = string.Empty;
        }

        public void Closed(bool flag)
        {
            ClosePort();
        }
    }
}
