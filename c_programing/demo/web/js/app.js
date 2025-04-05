document.addEventListener("DOMContentLoaded", function () {
  // Get all demo items
  const demoItems = document.querySelectorAll(".demo-item");
  const outputElement = document.getElementById("output");

  console.log("DOM loaded, found", demoItems.length, "demo items");

  // Add click event to run buttons
  demoItems.forEach((item) => {
    const runButton = item.querySelector(".run-button");
    const demoName = item.getAttribute("data-demo");

    runButton.addEventListener("click", () => {
      // Show loading state
      outputElement.textContent = "Running demo...";
      document.querySelector(".output-container").classList.add("loading");

      console.log("Running demo:", demoName);

      // Run the demo via AJAX
      fetch(`/run/${demoName}`)
        .then((response) => {
          if (!response.ok) {
            throw new Error(`HTTP error! Status: ${response.status}`);
          }
          return response.json();
        })
        .then((data) => {
          console.log("Got response:", data);

          // Process the output to convert escaped sequences
          if (data.output) {
            // First replace all escaped newlines with actual newlines
            let formattedOutput = data.output
              .replace(/\\n/g, "\n")
              .replace(/\\t/g, "\t")
              .replace(/\\"/g, '"')
              .replace(/\\\\/g, "\\");

            console.log("Processed output, length:", formattedOutput.length);

            // Set the output text
            outputElement.textContent = formattedOutput;

            // Auto-scroll to the bottom of the output
            outputElement.scrollTop = outputElement.scrollHeight;
          } else {
            outputElement.textContent = "No output received from demo.";
          }
        })
        .catch((error) => {
          console.error("Error:", error);
          outputElement.textContent = `Error executing demo: ${error.message}`;
        })
        .finally(() => {
          // Remove loading state
          document.querySelector(".output-container").classList.remove("loading");
        });
    });
  });
});
