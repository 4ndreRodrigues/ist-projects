package m19.app.users;

import pt.tecnico.po.ui.Command;
import m19.LibraryManager;
import m19.User;

/**
 * 4.2.4. Show all users.
 */
public class DoShowUserWithMostRequests extends Command<LibraryManager> {

	/**
	 * @param receiver
	 */
	public DoShowUserWithMostRequests(LibraryManager receiver) {
		super(Label.SHOW_USER_WITH_MOST_REQUESTS, receiver);
	}

	/** @see pt.tecnico.po.ui.Command#execute() */
	@Override
	public final void execute() {
		User user = _receiver.getUserWithMostRequests();
        _display.addLine(user.toString());
		_display.display();
	}
}
