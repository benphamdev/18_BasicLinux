document.addEventListener("DOMContentLoaded", () => {
  // Select relevant DOM elements
  const demoItems = document.querySelectorAll(".demo-item");
  const outputElement = document.getElementById("output");
  const outputContainer = document.querySelector(".output-container"); // Cache container

  console.log(`DOM loaded, found ${demoItems.length} demo items.`);

  // --- Helper Function to Format Output ---
  const formatOutput = (rawOutput) => {
    if (!rawOutput) return "No output received from demo.";
    // Process escaped sequences for display in <pre> tag
    return rawOutput
      .replace(/\\n/g, "\n") // Newlines
      .replace(/\\t/g, "\t") // Tabs
      .replace(/\\"/g, '"') // Double quotes
      .replace(/\\\\/g, "\\"); // Backslashes
  };

  // --- Helper Function to Handle Demo Execution ---
  const runDemo = async (demoName) => {
    console.log(`Running demo: ${demoName}`);
    // Show loading state
    outputElement.textContent = "Executing demo, please wait..."; // More informative text
    outputContainer.classList.add("loading");

    try {
      // Fetch demo results using async/await
      const response = await fetch(`/run/${demoName}`);

      if (!response.ok) {
        // Handle HTTP errors (e.g., 404, 500)
        throw new Error(`Server error: ${response.status} ${response.statusText}`);
      }

      const data = await response.json();
      console.log("Received response:", data);

      // Format and display the output
      const formattedOutput = formatOutput(data.output);
      outputElement.textContent = formattedOutput;

      // Auto-scroll to the bottom of the output
      outputElement.scrollTop = outputElement.scrollHeight;
    } catch (error) {
      // Handle network errors or JSON parsing errors
      console.error("Error running demo:", error);
      outputElement.textContent = `Error: Failed to run demo '${demoName}'.\n\nDetails: ${error.message}\n\nPlease check the server logs or network connection.`;
      outputElement.classList.add("output-important"); // Add error styling
    } finally {
      // Always remove loading state, regardless of success or failure
      outputContainer.classList.remove("loading");
    }
  };

  // --- Attach Event Listeners ---
  demoItems.forEach((item) => {
    const runButton = item.querySelector(".run-button");
    const demoName = item.getAttribute("data-demo");

    if (runButton && demoName) {
      runButton.addEventListener("click", () => {
        // Clear previous error styling if any
        outputElement.classList.remove("output-important");
        // Execute the demo
        runDemo(demoName);
      });
    } else {
      console.warn("Could not find run button or demo name for item:", item);
    }
  });

  // Initial message
  if (demoItems.length > 0) {
    outputElement.textContent = "Select a demo from the list and click 'Run Demo'.";
  } else {
    outputElement.textContent = "No demos available.";
  }
});
