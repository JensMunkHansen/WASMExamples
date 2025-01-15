const { exec } = require('child_process');
const fs = require('fs');

const SERVER_PID_FILE = './serve.pid';

// Start the server and save its PID
function startServer() {
  const serverCommand = 'npx serve -l 3000 build/Debug';
  const serverProcess = exec(serverCommand);

  // Save the server PID to a file
  fs.writeFileSync(SERVER_PID_FILE, serverProcess.pid.toString(), 'utf8');

  console.log(`Server started with PID: ${serverProcess.pid}`);

  serverProcess.stdout.on('data', (data) => console.log(data.toString()));
  serverProcess.stderr.on('data', (data) => console.error(data.toString()));
}

// Stop the server using the PID file
function stopServer() {
  if (fs.existsSync(SERVER_PID_FILE)) {
    const pid = parseInt(fs.readFileSync(SERVER_PID_FILE, 'utf8'), 10);
    try {
      process.kill(pid);
      fs.unlinkSync(SERVER_PID_FILE);
      console.log(`Server with PID ${pid} stopped successfully.`);
    } catch (err) {
      console.error(`Failed to stop server with PID ${pid}: ${err.message}`);
    }
  } else {
    console.log('No running server found.');
  }
}

// Parse command-line arguments
const command = process.argv[2];
if (command === 'start') {
  startServer();
} else if (command === 'stop') {
  stopServer();
} else {
  console.log('Usage: node serverManager.js [start|stop]');
}
