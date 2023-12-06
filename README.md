# REST API library/reader-subscriber/book web server 📚✨

Welcome to LibraConnect, your gateway to a world of literary wonders! This repository showcases the fantastic Homework 4 project, where we've created a seamless client-server interaction for a library system.

## Project Inspiration 🌟
Built upon the foundation of lab9 from the PCOM course, LibraConnect is a labor of inspiration and innovation. We've added our touch, introducing a `compute_delete_request` function and refining request types to seamlessly integrate JWT tokens. The magic of JSON object parsing comes alive with the Parson library by Kgabis, adding an extra layer of brilliance to the project.

## Dive into Testing 🚀
Curious minds, rejoice! Explore the journey of LibraConnect through hands-on testing.

## Unveiling the Code Symphony 🎶
### Main Function
Embark on the adventure with the `main` function, orchestrating the program's dance of variables, memory allocation, user command processing, and graceful termination.

### Function: code
The heartbeat of LibraConnect, the `code` function, translates command strings into a symphony of command codes. A harmonious dance of commands awaits!

### Function: remove_whitespace
Join the rhythm of the `remove_whitespace` function, elegantly sweeping away unnecessary characters and creating a clean canvas for commands.

### Function: extract_cookie_elements
Watch as `extract_cookie_elements` gracefully separates cookie elements, ensuring a delightful array of tokens for smooth interactions.

### Function: get_last_line
Step into the spotlight with `get_last_line`, stealing the scene as it finds the last line containing a target substring – a true hero in the search for relevance.

### Function: print_fields
A crescendo of elegance, the `print_fields` function serenades the user with JSON array melodies, printing the enchanting values of selected fields.

### Exit Handler: A Graceful Finale
In the grand finale, the `exit_handler` gracefully exits the stage, leaving behind the echo of "204 - No Content." A standing ovation for a successful performance!

## User Commands: A Tale of Interaction 📜
- **register_handler**: A saga of user registration, where characters come to life with prompts for username and password, creating a JSON object that sets the stage for a unique tale.

- **login_handler**: Witness the protagonist's journey through user login, overcoming challenges, and acquiring the precious JWT token – a key to the library kingdom.

- **enter_handler**: A mystical journey into the restricted library area, where the protagonist's authorization is put to the test, and the coveted JWT token unfolds the story.

- **get1_handler**: A delightful quest to retrieve a list of books, where the protagonist's authorization and token become the magic carpet for exploring the library's literary treasures.

- **get2_handler**: A personalized adventure to retrieve details about a specific book, unfolding a unique chapter for each book ID entered by the user.

- **add_handler**: The protagonist's creative journey to add a new book to the library, overcoming challenges and contributing to the expanding universe of literary works.

- **delete_handler**: A dramatic turn in the storyline as the protagonist faces the challenge of removing a book from the library, with authorization and book ID playing pivotal roles.

- **logout_handler**: The hero's graceful exit from the library, bidding farewell with a successful logout and a promise of more adventures to come.

## Join the LibraConnect Symphony! 🎭
LibraConnect invites you to witness the magic, contribute to the story, and explore the vast world of possibilities in the realm of client-server interactions. Let the adventure begin! 🚀
