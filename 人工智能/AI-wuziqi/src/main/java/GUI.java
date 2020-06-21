import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.awt.image.BufferedImage;

//界面的搭建：ActionListener动作事件监听器
public final class GUI implements ActionListener, MouseMotionListener, WindowListener {
        // 常量
        private static final String TITLE              = "五子棋小游戏";
        private static final int DELAY                 = 10;     // 延时delay between repaints in millis
        private static final int MAX_SIZE              = 10000;   // max width/height
        private static int instances = 0;
        private static Thread shutdownThread = null;
        // 控件
        private int width, height;             // 窗口 frame 的大小
        private JFrame frame;                  // 总窗口的 frame
        private JPanel panel;                  // 总的画布面板
        private ImagePanel imagePanel;         // 真正的绘画面板
        private BufferedImage image;           // 记录绘图的情况
        private Graphics2D g2;                 // 2D绘图 graphics context
        private JLabel statusBar;              // 状态栏显示鼠标移动的位置
        private Timer timer;                   // 绘制的动画时间
        private Color backgroundColor = Color.WHITE;
        private boolean PRETTY = true;         // 消除锯齿操作true to anti-alias
        private int currentZoom = 1;         //当前缩放倍数
        private int initialPixel;              // 初始化每个像素点

    // 根据width和height绘制一个panel
    public GUI(int width, int height) {
        if (width < 0 || width > MAX_SIZE || height < 0 || height > MAX_SIZE) {
            throw new IllegalArgumentException("Illegal width/height: " + width + " x " + height);
        }
        //synchronized保证线程同步
        synchronized (getClass()) {
            instances++;
            if (shutdownThread == null) {
                shutdownThread = new Thread(new Runnable() {
                    public void run() {
                        try {
                            while (true) {
                                //完成执行主线程已经挂掉
                                if (instances == 0&& !mainIsActive()) {
                                    try {
                                        System.exit(0);
                                    } catch (SecurityException sex) {}
                                }
                                Thread.sleep(250);
                            }
                        } catch (Exception e) {}
                    }
                });
                shutdownThread.setPriority(Thread.MIN_PRIORITY);
                shutdownThread.start();
            }
        }
        this.width = width;
        this.height = height;
        image = new BufferedImage(width, height, BufferedImage.TYPE_INT_ARGB);
        initialPixel = image.getRGB(0, 0);
        g2 = (Graphics2D) image.getGraphics();
        g2.setColor(Color.BLACK);
        if (PRETTY) {
            g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
        }

            try {
                UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
            } catch (Exception e) {}

            statusBar = new JLabel(" ");
            statusBar.setBorder(BorderFactory.createLineBorder(Color.BLACK));

            panel = new JPanel(new FlowLayout(FlowLayout.LEFT, 0, 0));
            panel.setBackground(backgroundColor);
            panel.setPreferredSize(new Dimension(width, height));
            imagePanel = new ImagePanel(image);
            imagePanel.setBackground(backgroundColor);
            panel.add(imagePanel);

            // 监听鼠标事件
            panel.addMouseMotionListener(this);

            // 主界面窗格
            frame = new JFrame(TITLE);
            frame.addWindowListener(this);
            JScrollPane center = new JScrollPane(panel);
            frame.getContentPane().add(center);
            frame.getContentPane().add(statusBar, "South");
            frame.setBackground(Color.WHITE);

            // 菜单栏
            setupMenuBar();

            frame.pack();
            center(frame);
            frame.setVisible(true);

            // 重绘update
            timer = new Timer(DELAY, this);
            timer.start();

    }
    // 自定义一个Panel面板
    private class ImagePanel extends JPanel {
        private static final long serialVersionUID = 0;
        private Image image;
        public ImagePanel(Image image) {
            setImage(image);
            setBackground(Color.WHITE);
            setPreferredSize(new Dimension(image.getWidth(this), image.getHeight(this)));//设置窗口大小
            setAlignmentX(0.0f);//设置垂直对齐方式
        }
        //设置背景颜色
        public void paintComponent(Graphics g) {
            super.paintComponent(g);
            Graphics2D g2 = (Graphics2D) g;
            //调整缩放的因子为1
            if (currentZoom != 1) {
                g2.scale(currentZoom, currentZoom);
            }
            //渲染图像，在绘制之前将图像空间的变换应用到用户空间。
            g2.drawImage(image, 0, 0, this);
        }

        public void setImage(Image image) {
            this.image = image;
            repaint();
        }
    }
    //ActionListener接口方法，初始化UI上选项栏组件，系统调用
    public void actionPerformed(ActionEvent e) {
        if (e.getSource() instanceof Timer) {
            // 重绘
            panel.repaint();
        } else if (e.getActionCommand().equals("退出")) {
            exit();
        } else if (e.getActionCommand().equals("放大")) {
            zoom(currentZoom + 1);
        } else if (e.getActionCommand().equals("缩小")) {
            zoom(currentZoom - 1);
        } else if (e.getActionCommand().equals("正常大小 (100%)")) {
            zoom(1);
        }
    }
    public void addMouseListener(MouseListener listener) {
        panel.addMouseListener(listener);
    }
        // 返回主线程是否在运行 main is active
        private static boolean mainIsActive() {
            ThreadGroup group = Thread.currentThread().getThreadGroup();
            int activeCount = group.activeCount();

            // 在线程组中寻找主线程
            Thread[] threads = new Thread[activeCount];
            group.enumerate(threads);
            for (int i = 0; i < threads.length; i++) {
                Thread thread = threads[i];
                String name = ("" + thread.getName()).toLowerCase();
                if (name.indexOf("main") >= 0 ||
                        name.indexOf("testrunner-assignmentrunner") >= 0) {
                    return thread.isAlive();
                }
            }

            // 没有找到主线程
            return false;
        }


        // 清除所有的线/颜色
        public void clear() {
            int[] pixels = new int[width * height];
            for (int i = 0; i < pixels.length; i++) {
                pixels[i] = initialPixel;
            }
            image.setRGB(0, 0, width, height, pixels, 0, 1);
        }


        // 监听鼠标行为并将坐标显示在statusbar上
        public void mouseDragged(MouseEvent e) {}
        public void mouseMoved(MouseEvent e) {
            int x = e.getX() / currentZoom;
            int y = e.getY() / currentZoom;
            setStatusBarText("(" + x + ", " + y + ")");
        }

        // 设置背景颜色
        public void setBackground(Color c) {
            backgroundColor = c;
                panel.setBackground(c);
                imagePanel.setBackground(c);
        }


        // 关闭程序并退出
        public void windowClosing(WindowEvent event) {
            frame.setVisible(false);
            synchronized (getClass()) {
                instances--;
            }
            frame.dispose();
        }
        // WindowListener接口方法
        public void windowActivated(WindowEvent event) {}
        public void windowClosed(WindowEvent event) {}
        public void windowDeactivated(WindowEvent event) {}
        public void windowDeiconified(WindowEvent event) {}
        public void windowIconified(WindowEvent event) {}
        public void windowOpened(WindowEvent event) {}

        // 根据factor进行放大缩小
        // factor >= 1
        public void zoom(int zoomFactor) {
            currentZoom = Math.max(1, zoomFactor);
                Dimension size = new Dimension(width * currentZoom, height * currentZoom);
                imagePanel.setPreferredSize(size);
                panel.setPreferredSize(size);
                imagePanel.validate();
                imagePanel.revalidate();
                panel.validate();
                panel.revalidate();
                frame.getContentPane().validate();
                imagePanel.repaint();
                setStatusBarText(" ");
                // resize
                Dimension screen = Toolkit.getDefaultToolkit().getScreenSize();
                if (size.width <= screen.width || size.height <= screen.height) {
                    frame.pack();
                }
            }


        // 把主窗口放到屏幕中间
        private void center(Window frame) {
            Toolkit tk = Toolkit.getDefaultToolkit();
            Dimension screen = tk.getScreenSize();

            int x = Math.max(0, (screen.width - frame.getWidth()) / 2);
            int y = Math.max(0, (screen.height - frame.getHeight()) / 2);
            frame.setLocation(x, y);
        }
        // 退出程序
        private void exit() {
            try {
                System.exit(0);
            } catch (SecurityException e) {
            }
        }

        // 底部的状态栏显示放大倍数
        private void setStatusBarText(String text) {
            if (currentZoom != 1) {
                text += " (current zoom: " + currentZoom + "x" + ")";
            }
            statusBar.setText(text);
        }

        // 初始化UI控件
        private void setupMenuBar() {
            JMenuItem zoomIn = new JMenuItem("放大", 'I');
            zoomIn.addActionListener(this);
            zoomIn.setAccelerator(KeyStroke.getKeyStroke("ctrl EQUALS"));

            JMenuItem zoomOut = new JMenuItem("缩小", 'O');
            zoomOut.addActionListener(this);
            zoomOut.setAccelerator(KeyStroke.getKeyStroke("ctrl MINUS"));

            JMenuItem zoomNormal = new JMenuItem("正常大小 (100%)", 'N');
            zoomNormal.addActionListener(this);
            zoomNormal.setAccelerator(KeyStroke.getKeyStroke("ctrl 0"));


            JMenuItem exit = new JMenuItem("退出", 'x');
            exit.addActionListener(this);

            JMenuItem about = new JMenuItem("关于", 'A');
            about.addActionListener(this);

            JMenu file = new JMenu("文件");
            file.setMnemonic('F');
            file.add(exit);
            JMenu view = new JMenu("视图");
            view.setMnemonic('V');
            view.add(zoomIn);
            view.add(zoomOut);
            view.add(zoomNormal);
            JMenuBar bar = new JMenuBar();
            bar.add(file);
            bar.add(view);
            frame.setJMenuBar(bar);
        }
    // 获得Graphics2D对象
    public Graphics2D getGraphics() {
        return g2;
    }

    // 返回Height
    public int getHeight() {
        return height;
    }

    // 用Dimension对象返回width和height
    public Dimension getSize() {
        return new Dimension(width, height);
    }

    // 返回Width
    public int getWidth() {
        return width;
    }

    // 返回目前的缩放倍数
    public int getZoom() {
        return currentZoom;
    }
    // 通过给定值height 程序必须再次调用getGraphics()，重新获取上下文来进行绘图
    public void setHeight(int height) {
        setSize(getWidth(), height);
    }
    public void setSize(int width, int height) {
        // 替换绘图的BufferedImage
        BufferedImage newImage = new BufferedImage(width, height, image.getType());
        imagePanel.setImage(newImage);
        newImage.getGraphics().drawImage(image, 0, 0, imagePanel);
        this.width = width;
        this.height = height;
        image = newImage;
        g2 = (Graphics2D) newImage.getGraphics();
        g2.setColor(Color.BLACK);
        if (PRETTY) {
            g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
        }
        zoom(currentZoom);

    }

}
